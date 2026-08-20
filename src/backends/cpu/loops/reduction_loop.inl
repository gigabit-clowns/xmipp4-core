// SPDX-License-Identifier: GPL-3.0-only

#include "reduction_loop.hpp"

#include "inner_loop_stride_dispatch.hpp"
#include "loop_schedule.hpp"
#include "parallel_grain.hpp"
#include "../config.hpp"

#include <xmipp4/core/layout/joint_cursor.hpp>
#include <xmipp4/core/platform/constexpr.hpp>
#include <xmipp4/core/platform/attributes.hpp>

#include <algorithm>
#include <memory>
#include <stdexcept>
#include <utility>

namespace xmipp4
{
namespace cpu
{

template <>
struct accumulator_footprint<type_list<>>
	: std::integral_constant<std::size_t, 0>
{
};

template <typename Head, typename... Tail>
struct accumulator_footprint<type_list<Head, Tail...>>
	: std::integral_constant<
		std::size_t,
		sizeof(Head) + accumulator_footprint<type_list<Tail...>>::value
	>
{
};

namespace detail
{

// How much of a tile the accumulators may take, and how long the tile may
// be as a result. See config.hpp for what the defaults are worth and why.
XMIPP4_CONST_CONSTEXPR std::size_t reduction_tile_budget =
	XMIPP4_REDUCTION_TILE_BUDGET;
XMIPP4_CONST_CONSTEXPR std::size_t minimum_reduction_tile =
	XMIPP4_MINIMUM_REDUCTION_TILE_SIZE;
XMIPP4_CONST_CONSTEXPR std::size_t maximum_reduction_tile =
	XMIPP4_MAXIMUM_REDUCTION_TILE_SIZE;

XMIPP4_INLINE_CONSTEXPR
std::size_t clamp_reduction_tile(std::size_t count) noexcept
{
	if (count > maximum_reduction_tile)
	{
		return maximum_reduction_tile;
	}

	if (count < minimum_reduction_tile)
	{
		return minimum_reduction_tile;
	}

	return count;
}

/**
 * @brief Detect a kernel's optional identity member.
 */
template <typename Kernel, typename... Accumulators>
class identity_detector
{
	template <typename K>
	static auto probe(int) -> decltype(
		std::declval<const K&>().identity(
			std::declval<Accumulators&>()...
		),
		std::true_type()
	);

	template <typename K>
	static std::false_type probe(...);

public:
	using type = decltype(probe<Kernel>(0));
};

/**
 * @brief One fixed size array per accumulator type.
 *
 * Separate arrays rather than one array of a composite accumulator: the tile
 * loop walks the accumulators of consecutive outputs, so keeping each
 * accumulator contiguous is what lets that loop vectorize.
 */
template <typename Accumulators, std::size_t Size>
struct accumulator_tiles;

template <typename... Accumulators, std::size_t Size>
struct accumulator_tiles<type_list<Accumulators...>, Size>
{
	using type = std::tuple<std::array<Accumulators, Size>...>;
};

/**
 * @brief The half-open slice of the reduced space one chunk folds.
 *
 * The first `count % chunk_count` chunks are one element longer than the
 * rest, which balances them without ever leaving one empty. An empty slice
 * would have no first element to seed from.
 */
inline
void split_reduced_range(
	std::size_t count,
	std::size_t chunk_count,
	std::size_t chunk,
	std::size_t &begin,
	std::size_t &end
) noexcept
{
	const auto quotient = count / chunk_count;
	const auto remainder = count % chunk_count;

	begin = chunk*quotient + std::min(chunk, remainder);
	end = begin + quotient;
	if (chunk < remainder)
	{
		++end;
	}
}

/**
 * @brief One buffer per accumulator type, holding the partial answers every
 * thread of a fold split arrived at.
 *
 * Separate buffers rather than one of a composite accumulator, for the reason
 * @ref accumulator_tiles keeps them apart: the merge walks the accumulators of
 * consecutive outputs.
 *
 * Sized at run time, unlike a tile, because it holds one entry per surviving
 * element per chunk. This is only ever built when the surviving space is too
 * small to share out, so that product stays small.
 */
template <typename Accumulators>
struct accumulator_buffers;

template <typename... Accumulators>
struct accumulator_buffers<type_list<Accumulators...>>
{
	// An array rather than a std::vector, because std::vector<bool> packs its
	// elements into bits and a reduction over booleans is an ordinary thing
	// to ask for. Two threads writing what look like distinct entries would
	// then be writing the same byte, which is a race the language gives no
	// way to see and no way to fix from here.
	using type = std::tuple<std::unique_ptr<Accumulators[]>...>;
};

/**
 * @brief Give every buffer the same length.
 *
 * Indexed rather than typed: two accumulators of a kernel may well be of the
 * same type, as the position and the value of an extremum are, and std::get
 * by type would not know them apart.
 */
template <typename... Accumulators, std::size_t... As>
inline
void resize_accumulator_buffers(
	std::tuple<std::unique_ptr<Accumulators[]>...> &buffers,
	std::size_t size,
	std::index_sequence<As...>
)
{
	(void) std::initializer_list<int> {
		(
			std::get<As>(buffers) =
				std::unique_ptr<Accumulators[]>(new Accumulators[size]()),
			0
		)...
	};
}

template <typename... Accumulators>
inline
void resize_accumulator_buffers(
	std::tuple<std::unique_ptr<Accumulators[]>...> &buffers,
	std::size_t size
)
{
	resize_accumulator_buffers(
		buffers,
		size,
		std::index_sequence_for<Accumulators...>()
	);
}

/**
 * @brief Stride of an operand along the innermost axis of a layout.
 *
 * A layout of rank zero holds a single position and no stride to read, which
 * is the shape a reduction over every axis leaves behind.
 */
inline
std::ptrdiff_t inner_stride(
	const joint_layout &layout,
	std::size_t operand
)
{
	const auto strides = layout.get_strides(operand);
	return strides.empty() ? 0 : strides[0];
}

/**
 * @brief Displace every pointer of a tuple by one offset each.
 */
template <typename... Pointers, std::size_t... Is>
inline
std::tuple<Pointers...> offset_pointers(
	const std::tuple<Pointers...> &pointers,
	const std::ptrdiff_t *offsets,
	std::index_sequence<Is...>
)
{
	return std::make_tuple((std::get<Is>(pointers) + offsets[Is])...);
}

/**
 * @brief Advance every pointer of a tuple by a multiple of its stride.
 */
template <typename... Pointers, typename... Strides, std::size_t... Is>
inline
std::tuple<Pointers...> step_pointers(
	const std::tuple<Pointers...> &pointers,
	std::size_t index,
	const std::tuple<Strides...> &strides,
	std::index_sequence<Is...>
)
{
	const auto step = static_cast<std::ptrdiff_t>(index);
	return std::make_tuple(
		(
			std::get<Is>(pointers) +
			step * static_cast<std::ptrdiff_t>(std::get<Is>(strides))
		)...
	);
}

template <typename... Pointers, std::size_t Count, std::size_t... Is>
inline
std::tuple<Pointers...> step_pointers(
	const std::tuple<Pointers...> &pointers,
	std::size_t index,
	const std::array<std::ptrdiff_t, Count> &strides,
	std::index_sequence<Is...>
)
{
	const auto step = static_cast<std::ptrdiff_t>(index);
	return std::make_tuple(
		(std::get<Is>(pointers) + step * strides[Is])...
	);
}

/**
 * @brief Drives a reduction over a planned iteration.
 *
 * The traversal is a class rather than a function so that the state every
 * level of the nest needs, the layouts, the cursors, the resolved strides and
 * the accumulator tiles, is named once and read by name, instead of being
 * threaded through argument lists that grow with every level.
 *
 * The nest has three levels, one member each: @ref run walks the surviving
 * space, @ref run_kept_vector cuts each of its 1D vectors into tiles, and
 * @ref accumulate_tile folds the whole reduced space into one tile before
 * @ref finalize_tile writes it out.
 */
template <
	typename Kernel,
	typename Outputs,
	typename Inputs,
	typename KeptStrides,
	typename ReducedStrides
>
class reduction_loop_runner;

template <
	typename Kernel,
	typename... Outs,
	typename... Ins,
	typename... KeptStrides,
	typename... ReducedStrides
>
class reduction_loop_runner<
	Kernel,
	std::tuple<Outs*...>,
	std::tuple<const Ins*...>,
	std::tuple<KeptStrides...>,
	std::tuple<ReducedStrides...>
>
{
public:
	using output_pointers = std::tuple<Outs*...>;
	using input_pointers = std::tuple<const Ins*...>;
	using kept_strides = std::tuple<KeptStrides...>;
	using reduced_strides = std::tuple<ReducedStrides...>;

	using accumulator_types = typename Kernel::template accumulators<
		type_list<Outs...>,
		type_list<Ins...>
	>::type;

	static_assert(
		type_list_size<accumulator_types>::value > 0,
		"A reduction kernel must declare at least one accumulator."
	);

	static XMIPP4_CONST_CONSTEXPR std::size_t input_count = sizeof...(Ins);
	static XMIPP4_CONST_CONSTEXPR std::size_t output_count = sizeof...(Outs);
	static XMIPP4_CONST_CONSTEXPR std::size_t tile_size =
		reduction_tile_size<accumulator_types>::value;

	using tiles_type =
		typename accumulator_tiles<accumulator_types, tile_size>::type;
	using partial_buffers =
		typename accumulator_buffers<accumulator_types>::type;
	using accumulator_indices = std::make_index_sequence<
		type_list_size<accumulator_types>::value
	>;
	using input_indices = std::index_sequence_for<Ins...>;
	using output_indices = std::index_sequence_for<Outs...>;
	using identity_available =
		typename has_reduction_identity<Kernel, accumulator_types>::type;

	// Whether the tile belongs outside the fold, which it does when the axis
	// being folded is the contiguous one and the surviving axis is not. The
	// first input decides: the others rarely disagree, and none of them is
	// read more often than it.
	using tile_outermost = std::integral_constant<
		bool,
		std::is_same<
			typename std::tuple_element<0, reduced_strides>::type,
			contiguous_stride_tag
		>::value &&
		!std::is_same<
			typename std::tuple_element<0, kept_strides>::type,
			contiguous_stride_tag
		>::value
	>;

	reduction_loop_runner(
		const Kernel &kernel,
		const joint_layout &kept_layout,
		const joint_layout &reduced_layout,
		std::size_t reduction_count,
		const output_pointers &outputs,
		const input_pointers &inputs,
		const kept_strides &kept_inner_strides,
		const reduced_strides &reduced_inner_strides
	)
		: m_kernel(kernel)
		, m_kept_layout(kept_layout)
		, m_reduced_layout(reduced_layout)
		, m_reduction_count(reduction_count)
		, m_outputs(outputs)
		, m_inputs(inputs)
		, m_kept_strides(kept_inner_strides)
		, m_reduced_strides(reduced_inner_strides)
		, m_reduced_vector_size(0)
	{
		// The outputs are operands of the kept layout, after the inputs.
		for (std::size_t i = 0; i < output_count; ++i)
		{
			m_output_strides[i] = inner_stride(kept_layout, input_count + i);
		}
	}

	/**
	 * @brief Reduce into a range of the output elements.
	 *
	 * The range is over the surviving space, so it names a set of outputs.
	 * Two runners covering a range between them fill exactly the outputs one
	 * runner over the whole of it fills, each output folded by exactly one of
	 * them and in the order it would be folded serially. That is what lets
	 * the surviving space be shared out without the kernel having to merge
	 * anything.
	 *
	 * @param begin First position of the surviving space to fill.
	 * @param end Past-the-end position of the surviving space to fill.
	 */
	void run_range(std::size_t begin, std::size_t end)
	{
		// A full sweep of the reduced space rewinds its cursor, so it is
		// armed once here and reused by every tile. Arming it per tile would
		// allocate once per tile, iter() building a fresh cursor every call.
		m_reduced_vector_size = m_reduced_layout.iter(m_reduced_cursor);

		walk_kept_range(
			begin,
			end,
			[this] (
				const input_pointers &inputs,
				const output_pointers &outputs,
				std::size_t /*kept_position*/,
				std::size_t width
			)
			{
				accumulate_tile(inputs, width);
				finalize_tile(outputs, width);
			}
		);
	}

	/**
	 * @brief Fold one slice of the reduced space into a set of partials.
	 *
	 * Walks the whole surviving space, folding only `[begin, end)` of the
	 * reduced space into each tile, and leaves the accumulators in @p buffers
	 * rather than finishing them into outputs. What every chunk leaves there
	 * is merged afterwards by @ref merge_and_finalize.
	 *
	 * The accumulators are built in the runner's own tile and copied out once
	 * per tile, rather than accumulated into @p buffers directly. With a
	 * surviving space small enough for this to be the path taken, several
	 * chunks' entries share a cache line, and folding into them would be
	 * false sharing on the hottest variable of the loop.
	 *
	 * @param chunk Which chunk this is, deciding where in @p buffers it
	 * writes.
	 * @param kept_count How many surviving elements there are.
	 * @param begin,end The slice of the reduced space to fold.
	 * @param buffers Where the partial accumulators are left.
	 */
	void fold_reduced_slice(
		std::size_t chunk,
		std::size_t kept_count,
		std::size_t begin,
		std::size_t end,
		partial_buffers &buffers
	)
	{
		XMIPP4_ASSERT( begin < end );

		// Where this slice starts. Every tile re-folds the same slice, so the
		// cursor is positioned once and copied back rather than sought again.
		m_reduced_vector_size = m_reduced_layout.seek(m_reduced_cursor, begin);
		XMIPP4_ASSERT( m_reduced_vector_size > 0 );
		m_reduced_origin = m_reduced_cursor;

		const auto base = chunk*kept_count;
		walk_kept_range(
			0,
			kept_count,
			[this, base, begin, end, &buffers] (
				const input_pointers &inputs,
				const output_pointers &/*outputs*/,
				std::size_t kept_position,
				std::size_t width
			)
			{
				accumulate_slice(inputs, width, begin, end);
				store_tile(buffers, base + kept_position, width);
			}
		);
	}

	/**
	 * @brief Merge every chunk's partials and write the outputs.
	 *
	 * Ascending in the chunk index, never in the order the chunks happened to
	 * finish: merge is only required to keep the earlier of two equally good
	 * answers, so the order it sees is what decides which one that is. Running
	 * serially here is what makes that order the reduced space's own, and the
	 * answer the same whichever thread did what.
	 *
	 * @param chunk_count How many chunks left partials.
	 * @param kept_count How many surviving elements there are.
	 * @param buffers The partial accumulators.
	 */
	void merge_and_finalize(
		std::size_t chunk_count,
		std::size_t kept_count,
		const partial_buffers &buffers
	)
	{
		walk_kept_range(
			0,
			kept_count,
			[this, chunk_count, kept_count, &buffers] (
				const input_pointers &/*inputs*/,
				const output_pointers &outputs,
				std::size_t kept_position,
				std::size_t width
			)
			{
				merge_tile(
					buffers,
					kept_position,
					kept_count,
					chunk_count,
					width
				);
				finalize_tile(outputs, width);
			}
		);
	}

private:
	/**
	 * @brief Walk a range of the surviving space, tile by tile.
	 *
	 * @param begin,end The range of the surviving space to walk.
	 * @param action Invoked per tile with the operand pointers displaced to
	 * its start, the tile's position in the surviving space, and its width.
	 */
	template <typename Action>
	void walk_kept_range(
		std::size_t begin,
		std::size_t end,
		const Action &action
	)
	{
		if (begin >= end)
		{
			return;
		}

		auto run = m_kept_layout.seek(m_kept_cursor, begin);
		if (run == 0)
		{
			return; // The outputs are empty, so there is nothing to fill.
		}

		auto position = begin;
		auto remaining = end - begin;
		for (;;)
		{
			// The cursor may be parked inside a vector, in which case what is
			// left of it is what gets walked.
			const auto vector_size = std::min(run, remaining);
			walk_kept_vector(position, vector_size, action);

			position += vector_size;
			remaining -= vector_size;
			if (remaining == 0)
			{
				break;
			}

			run = m_kept_layout.next(m_kept_cursor, vector_size);
			XMIPP4_ASSERT( run > 0 );
		}
	}


	/**
	 * @brief Cut one 1D vector of the surviving space into tiles.
	 *
	 * @param position Where the vector starts in the surviving space.
	 * @param vector_size How many of its elements to cover.
	 * @param action Invoked per tile.
	 */
	template <typename Action>
	void walk_kept_vector(
		std::size_t position,
		std::size_t vector_size,
		const Action &action
	)
	{
		const auto offsets = m_kept_cursor.get_offsets();
		const auto inputs = offset_pointers(
			m_inputs,
			offsets.data(),
			input_indices()
		);
		const auto outputs = offset_pointers(
			m_outputs,
			offsets.data() + input_count,
			output_indices()
		);

		for (std::size_t begin = 0; begin < vector_size; begin += tile_size)
		{
			const auto width = std::min(tile_size, vector_size - begin);

			action(
				step_pointers(inputs, begin, m_kept_strides, input_indices()),
				step_pointers(
					outputs,
					begin,
					m_output_strides,
					output_indices()
				),
				position + begin,
				width
			);
		}
	}

	/**
	 * @brief Fold the whole reduced space into the accumulators of one tile.
	 */
	void accumulate_tile(const input_pointers &inputs, std::size_t width)
	{
		if (m_reduced_vector_size == 0)
		{
			fill_with_identity(width, identity_available());
			return;
		}

		const auto offsets = m_reduced_cursor.get_offsets();
		const auto first = offset_pointers(
			inputs,
			offsets.data(),
			input_indices()
		);

		// Seeding from the first element, rather than from an identity, is
		// what lets a fold with no neutral element be expressed the same way
		// as one that has.
		auto count = m_reduced_vector_size;
		seed_tile(first, width, 0);

		// Where in the reduced space each element sits. Every tile sweeps
		// that space the same way, so the count restarts with each of them.
		std::size_t position = 1;
		combine_run(
			step_pointers(first, 1, m_reduced_strides, input_indices()),
			width,
			count - 1,
			position
		);
		position += count - 1;

		while ((count = m_reduced_layout.next(m_reduced_cursor, count)))
		{
			combine_run(
				offset_pointers(inputs, offsets.data(), input_indices()),
				width,
				count,
				position
			);
			position += count;
		}
	}

	/**
	 * @brief Fold one slice of the reduced space into the accumulators of a
	 * tile.
	 *
	 * Seeds from the first element of the slice, exactly as the whole-space
	 * fold seeds from the first element of the space, so a fold with no
	 * neutral element needs nothing extra here either.
	 *
	 * The positions handed to the kernel are absolute within the reduced
	 * space, not relative to the slice. An operation reporting where in that
	 * space it found something depends on it, and so does the merge that
	 * follows: the earlier of two equally good answers is only the earlier
	 * one if both were numbered from the same origin.
	 *
	 * @param inputs Operand pointers at the start of the tile.
	 * @param width How many outputs the tile covers.
	 * @param begin,end The slice of the reduced space to fold.
	 */
	void accumulate_slice(
		const input_pointers &inputs,
		std::size_t width,
		std::size_t begin,
		std::size_t end
	)
	{
		XMIPP4_ASSERT( begin < end );

		// Every tile folds the same slice, so the cursor goes back to where
		// the slice starts rather than being sought there again: seek()
		// allocates, an assignment between two cursors of one shape does not.
		m_reduced_cursor = m_reduced_origin;

		auto run = m_reduced_vector_size;
		auto remaining = end - begin;
		auto position = begin;

		{
			const auto offsets = m_reduced_cursor.get_offsets();
			const auto first = offset_pointers(
				inputs,
				offsets.data(),
				input_indices()
			);

			seed_tile(first, width, position);

			// The rest of the run the slice starts in, minus the element the
			// seed just consumed.
			const auto count = std::min(run, remaining) - 1;
			combine_run(
				step_pointers(first, 1, m_reduced_strides, input_indices()),
				width,
				count,
				position + 1
			);

			run = count + 1;
		}

		position += run;
		remaining -= run;

		while (remaining)
		{
			run = m_reduced_layout.next(m_reduced_cursor, run);
			XMIPP4_ASSERT( run > 0 );

			const auto offsets = m_reduced_cursor.get_offsets();
			const auto count = std::min(run, remaining);
			combine_run(
				offset_pointers(inputs, offsets.data(), input_indices()),
				width,
				count,
				position
			);

			run = count;
			position += count;
			remaining -= count;
		}
	}

	/**
	 * @brief Copy a tile's accumulators into a chunk's partials.
	 */
	void store_tile(
		partial_buffers &buffers,
		std::size_t slot,
		std::size_t width
	)
	{
		store_tile(buffers, slot, width, accumulator_indices());
	}

	template <std::size_t... As>
	void store_tile(
		partial_buffers &buffers,
		std::size_t slot,
		std::size_t width,
		std::index_sequence<As...>
	)
	{
		for (std::size_t j = 0; j < width; ++j)
		{
			(void) std::initializer_list<int> {
				(
					std::get<As>(buffers)[slot + j] =
						std::get<As>(m_tiles)[j],
					0
				)...
			};
		}
	}

	/**
	 * @brief Fold every chunk's partials for a tile into its accumulators.
	 */
	void merge_tile(
		const partial_buffers &buffers,
		std::size_t slot,
		std::size_t kept_count,
		std::size_t chunk_count,
		std::size_t width
	)
	{
		merge_tile(
			buffers,
			slot,
			kept_count,
			chunk_count,
			width,
			accumulator_indices()
		);
	}

	template <std::size_t... As>
	void merge_tile(
		const partial_buffers &buffers,
		std::size_t slot,
		std::size_t kept_count,
		std::size_t chunk_count,
		std::size_t width,
		std::index_sequence<As...>
	)
	{
		for (std::size_t j = 0; j < width; ++j)
		{
			// The first chunk is the answer so far, and every other is folded
			// into it in ascending order. Starting from it rather than from
			// an identity is the same reason seeding does.
			(void) std::initializer_list<int> {
				(
					std::get<As>(m_tiles)[j] =
						std::get<As>(buffers)[slot + j],
					0
				)...
			};

			for (std::size_t c = 1; c < chunk_count; ++c)
			{
				const auto other = c*kept_count + slot + j;
				m_kernel.merge(
					std::get<As>(m_tiles)[j]...,
					std::get<As>(buffers)[other]...
				);
			}
		}
	}

	void seed_tile(
		const input_pointers &inputs,
		std::size_t width,
		std::size_t position
	)
	{
		seed_tile(
			inputs,
			width,
			position,
			accumulator_indices(),
			input_indices()
		);
	}

	template <std::size_t... As, std::size_t... Is>
	void seed_tile(
		const input_pointers &inputs,
		std::size_t width,
		std::size_t position,
		std::index_sequence<As...>,
		std::index_sequence<Is...>
	)
	{
		for (std::size_t j = 0; j < width; ++j)
		{
			const auto step = static_cast<std::ptrdiff_t>(j);
			m_kernel.seed(
				std::get<As>(m_tiles)[j]...,
				(
					std::get<Is>(inputs) +
					step * static_cast<std::ptrdiff_t>(
						std::get<Is>(m_kept_strides)
					)
				)...,
				position
			);
		}
	}

	/**
	 * @brief Fold one 1D vector of the reduced space into a tile.
	 *
	 * Which of the two loops goes inside decides how the input is read, and
	 * the answer differs by orientation:
	 *
	 * - When the surviving axis is the contiguous one, the tile belongs
	 *   inside. Consecutive outputs and the elements feeding them are then
	 *   both walked contiguously, and the loop vectorizes across the tile.
	 * - When the axis being folded is the contiguous one, the tile belongs
	 *   outside. Each output's run of elements is then read as the
	 *   contiguous stream it is, into an accumulator that stays put for the
	 *   whole run. Keeping the tile inside here would turn that stream into
	 *   a gather one tile stride apart, which costs an order of magnitude.
	 *
	 * The strides are resolved to tags before either loop is instantiated,
	 * so the choice is made at compile time and neither shape carries a
	 * test for it.
	 */
	void combine_run(
		const input_pointers &inputs,
		std::size_t width,
		std::size_t count,
		std::size_t position
	)
	{
		combine_run(
			inputs,
			width,
			count,
			position,
			tile_outermost(),
			accumulator_indices(),
			input_indices()
		);
	}

	// The surviving axis is walked with a stride, the folded one without.
	template <std::size_t... As, std::size_t... Is>
	void combine_run(
		const input_pointers &inputs,
		std::size_t width,
		std::size_t count,
		std::size_t position,
		std::true_type,
		std::index_sequence<As...>,
		std::index_sequence<Is...>
	)
	{
		for (std::size_t j = 0; j < width; ++j)
		{
			const auto column = step_pointers(
				inputs,
				j,
				m_kept_strides,
				input_indices()
			);

			for (std::size_t e = 0; e < count; ++e)
			{
				const auto step = static_cast<std::ptrdiff_t>(e);
				m_kernel.combine(
					std::get<As>(m_tiles)[j]...,
					(
						std::get<Is>(column) +
						step * static_cast<std::ptrdiff_t>(
							std::get<Is>(m_reduced_strides)
						)
					)...,
					position + e
				);
			}
		}
	}

	// The folded axis is walked with a stride, the surviving one without.
	template <std::size_t... As, std::size_t... Is>
	void combine_run(
		const input_pointers &inputs,
		std::size_t width,
		std::size_t count,
		std::size_t position,
		std::false_type,
		std::index_sequence<As...>,
		std::index_sequence<Is...>
	)
	{
		for (std::size_t e = 0; e < count; ++e)
		{
			const auto row = step_pointers(
				inputs,
				e,
				m_reduced_strides,
				input_indices()
			);

			for (std::size_t j = 0; j < width; ++j)
			{
				const auto step = static_cast<std::ptrdiff_t>(j);
				m_kernel.combine(
					std::get<As>(m_tiles)[j]...,
					(
						std::get<Is>(row) +
						step * static_cast<std::ptrdiff_t>(
							std::get<Is>(m_kept_strides)
						)
					)...,
					position + e
				);
			}
		}
	}

	void finalize_tile(const output_pointers &outputs, std::size_t width)
	{
		finalize_tile(
			outputs,
			width,
			accumulator_indices(),
			output_indices()
		);
	}

	template <std::size_t... As, std::size_t... Os>
	void finalize_tile(
		const output_pointers &outputs,
		std::size_t width,
		std::index_sequence<As...>,
		std::index_sequence<Os...>
	)
	{
		for (std::size_t j = 0; j < width; ++j)
		{
			const auto step = static_cast<std::ptrdiff_t>(j);
			m_kernel.finalize(
				(std::get<Os>(outputs) + step * m_output_strides[Os])...,
				std::get<As>(m_tiles)[j]...,
				m_reduction_count
			);
		}
	}

	void fill_with_identity(std::size_t width, std::true_type)
	{
		fill_with_identity(width, accumulator_indices());
	}

	XMIPP4_NORETURN
	void fill_with_identity(std::size_t, std::false_type) const
	{
		throw std::invalid_argument(
			"run_reduction_loop: A reduction over no elements has no answer "
			"for an operation without an identity."
		);
	}

	template <std::size_t... As>
	void fill_with_identity(std::size_t width, std::index_sequence<As...>)
	{
		for (std::size_t j = 0; j < width; ++j)
		{
			m_kernel.identity(std::get<As>(m_tiles)[j]...);
		}
	}

	const Kernel &m_kernel;
	const joint_layout &m_kept_layout;
	const joint_layout &m_reduced_layout;
	std::size_t m_reduction_count;
	output_pointers m_outputs;
	input_pointers m_inputs;
	kept_strides m_kept_strides;
	reduced_strides m_reduced_strides;
	std::array<std::ptrdiff_t, output_count> m_output_strides;
	joint_cursor m_kept_cursor;
	joint_cursor m_reduced_cursor;
	joint_cursor m_reduced_origin;
	std::size_t m_reduced_vector_size;
	tiles_type m_tiles;
};

template <
	typename Kernel,
	typename Outputs,
	typename Inputs,
	typename KeptStrides,
	typename ReducedStrides
>
inline
void run_reduction_loop_runner(
	const Kernel &kernel,
	const joint_layout &kept_layout,
	const joint_layout &reduced_layout,
	std::size_t reduction_count,
	const Outputs &outputs,
	const Inputs &inputs,
	const KeptStrides &kept_strides,
	const ReducedStrides &reduced_strides,
	std::size_t begin,
	std::size_t end
)
{
	using runner_type = reduction_loop_runner<
		Kernel, 
		Outputs, 
		Inputs, 
		KeptStrides, 
		ReducedStrides
	>;

	runner_type runner(
		kernel,
		kept_layout,
		reduced_layout,
		reduction_count,
		outputs,
		inputs,
		kept_strides,
		reduced_strides
	);
	runner.run_range(begin, end);
}

/**
 * @brief Fold a reduction by splitting the space it folds over.
 *
 * Used when there are too few surviving elements to give every thread one.
 * Each chunk folds its own slice of the reduced space over the whole
 * surviving space into partials of its own, and those are merged afterwards,
 * in ascending chunk order, on the thread that joins them.
 *
 * The kernel's merge is what makes this expressible, and this is the only
 * caller of it.
 *
 * @warning Splitting the fold reassociates it. A sum of floating point values
 * therefore stops being bit for bit what one thread computes, and stops being
 * the same across worker counts.
 */
template <
	typename Kernel,
	typename Outputs,
	typename Inputs,
	typename KeptStrides,
	typename ReducedStrides
>
inline
void run_reduction_fold_split(
	const Kernel &kernel,
	const joint_layout &kept_layout,
	const joint_layout &reduced_layout,
	std::size_t reduction_count,
	const Outputs &outputs,
	const Inputs &inputs,
	const KeptStrides &kept_strides,
	const ReducedStrides &reduced_strides,
	std::size_t kept_count,
	std::size_t chunk_count,
	const loop_schedule &schedule
)
{
	using runner_type = reduction_loop_runner<
		Kernel,
		Outputs,
		Inputs,
		KeptStrides,
		ReducedStrides
	>;
	using buffers_type = typename runner_type::partial_buffers;

	// One entry per surviving element per chunk. This path is only taken when
	// the surviving space is too small to share out, so the product is small.
	buffers_type buffers;
	resize_accumulator_buffers(buffers, chunk_count*kept_count);

	// Cut here rather than letting the pool cut, so that a chunk index means
	// the same slice however many chunks the pool decides to run at once, and
	// so that the whole thing still works when it declines to split at all.
	schedule.with_grain(1).run(
		chunk_count,
		[&] (std::size_t first, std::size_t last)
		{
			for (std::size_t chunk = first; chunk < last; ++chunk)
			{
				std::size_t begin;
				std::size_t end;
				split_reduced_range(
					reduction_count,
					chunk_count,
					chunk,
					begin,
					end
				);

				runner_type runner(
					kernel,
					kept_layout,
					reduced_layout,
					reduction_count,
					outputs,
					inputs,
					kept_strides,
					reduced_strides
				);
				runner.fold_reduced_slice(
					chunk,
					kept_count,
					begin,
					end,
					buffers
				);
			}
		}
	);

	runner_type runner(
		kernel,
		kept_layout,
		reduced_layout,
		reduction_count,
		outputs,
		inputs,
		kept_strides,
		reduced_strides
	);
	runner.merge_and_finalize(chunk_count, kept_count, buffers);
}

} // namespace detail

template <typename Accumulators>
struct reduction_tile_size
	: std::integral_constant<
		std::size_t,
		detail::clamp_reduction_tile(
			detail::reduction_tile_budget /
			accumulator_footprint<Accumulators>::value
		)
	>
{
};

template <typename Kernel, typename... Accumulators>
struct has_reduction_identity<Kernel, type_list<Accumulators...>>
	: detail::identity_detector<Kernel, Accumulators...>::type
{
};

template <typename Kernel, typename... Outs, typename... Ins>
inline
void run_reduction_loop(
	const Kernel &kernel,
	const joint_layout &kept_layout,
	const joint_layout &reduced_layout,
	std::size_t reduction_count,
	const std::tuple<Outs*...> &outputs,
	const std::tuple<const Ins*...> &inputs
)
{
	run_reduction_loop(
		kernel,
		kept_layout,
		reduced_layout,
		reduction_count,
		outputs,
		inputs,
		loop_schedule()
	);
}

template <typename Kernel, typename... Outs, typename... Ins>
inline
void run_reduction_loop(
	const Kernel &kernel,
	const joint_layout &kept_layout,
	const joint_layout &reduced_layout,
	std::size_t reduction_count,
	const std::tuple<Outs*...> &outputs,
	const std::tuple<const Ins*...> &inputs,
	const loop_schedule &schedule
)
{
	XMIPP4_CONST_CONSTEXPR std::index_sequence_for<Ins...> input_indices {};

	// One iteration of the surviving space folds the whole reduced space, so
	// that is what an iteration costs and what the grain is stated in.
	const auto cost = reduction_count > 0 ? reduction_count : 1;
	const auto kept_count = kept_layout.compute_element_count();
	const auto workers = schedule.get_concurrency();

	// Which of the two spaces to share out. Splitting the surviving one is
	// preferred wherever it has the parallelism to spare: it needs no merge,
	// no buffer, and it reassociates nothing, so its answer is what one
	// thread computes down to the last bit. Twice the worker count rather
	// than once, so that a ragged finish has somewhere to go.
	//
	// Only when there are too few outputs for that is the fold itself split,
	// of which a reduction to a scalar is the extreme: one output, and all of
	// the work behind it.
	const auto split_the_fold =
		workers > 1 &&
		kept_count > 0 &&
		kept_count < 2*workers &&
		reduction_count >= 2*workers &&
		kept_count*reduction_count >= get_parallel_grain_size();

	if (split_the_fold)
	{
		const auto chunk_count = std::min(reduction_count, workers);

		dispatch_inner_loop_strides(
			[&] (auto kept_strides)
			{
				dispatch_inner_loop_strides(
					[&] (auto reduced_strides)
					{
						detail::run_reduction_fold_split(
							kernel,
							kept_layout,
							reduced_layout,
							reduction_count,
							outputs,
							inputs,
							kept_strides,
							reduced_strides,
							kept_count,
							chunk_count,
							schedule
						);
					},
					reduced_layout,
					input_indices
				);
			},
			kept_layout,
			input_indices
		);

		return;
	}

	// The split is outside both stride dispatches. Inside, the body handed to
	// the pool would be a distinct type per pair of stride combinations and
	// would instantiate the type erasure 3^(2N) times over. Outside, it is
	// instantiated once and the dispatches are re-entered per chunk.
	schedule.with_grain(grain_for_cost(cost)).run(
		kept_count,
		[&] (std::size_t begin, std::size_t end)
		{
			dispatch_inner_loop_strides(
				[&] (auto kept_strides)
				{
					dispatch_inner_loop_strides(
						[&] (auto reduced_strides)
						{
							// A runner of its own per chunk, so that the
							// cursors and the accumulator tiles it carries
							// live on the stack of whichever thread runs it.
							detail::run_reduction_loop_runner(
								kernel,
								kept_layout,
								reduced_layout,
								reduction_count,
								outputs,
								inputs,
								kept_strides,
								reduced_strides,
								begin,
								end
							);
						},
						reduced_layout,
						input_indices
					);
				},
				kept_layout,
				input_indices
			);
		}
	);
}

} // namespace cpu
} // namespace xmipp4
