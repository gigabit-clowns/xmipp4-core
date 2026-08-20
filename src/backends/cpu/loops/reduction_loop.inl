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
		if (begin >= end)
		{
			return;
		}

		auto run = m_kept_layout.seek(m_kept_cursor, begin);
		if (run == 0)
		{
			return; // The outputs are empty, so there is nothing to fill.
		}

		// A full sweep of the reduced space rewinds its cursor, so it is
		// armed once here and reused by every tile. Arming it per tile would
		// allocate once per tile, iter() building a fresh cursor every call.
		m_reduced_vector_size = m_reduced_layout.iter(m_reduced_cursor);

		auto remaining = end - begin;
		for (;;)
		{
			// run_kept_vector steps from wherever the cursor sits, so a
			// cursor parked inside a vector is handed what is left of it.
			const auto vector_size = std::min(run, remaining);
			run_kept_vector(vector_size);

			remaining -= vector_size;
			if (remaining == 0)
			{
				break;
			}

			run = m_kept_layout.next(m_kept_cursor, vector_size);
			XMIPP4_ASSERT( run > 0 );
		}
	}

private:
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

	/**
	 * @brief Complete every output of one 1D vector of the surviving space.
	 */
	void run_kept_vector(std::size_t vector_size)
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

			accumulate_tile(
				step_pointers(inputs, begin, m_kept_strides, input_indices()),
				width
			);
			finalize_tile(
				step_pointers(
					outputs,
					begin,
					m_output_strides,
					output_indices()
				),
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
		seed_tile(first, width);

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

	void seed_tile(const input_pointers &inputs, std::size_t width)
	{
		seed_tile(inputs, width, accumulator_indices(), input_indices());
	}

	template <std::size_t... As, std::size_t... Is>
	void seed_tile(
		const input_pointers &inputs,
		std::size_t width,
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
				std::size_t(0)
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

	// The split is outside both stride dispatches. Inside, the body handed to
	// the pool would be a distinct type per pair of stride combinations and
	// would instantiate the type erasure 3^(2N) times over. Outside, it is
	// instantiated once and the dispatches are re-entered per chunk.
	schedule.with_grain(grain_for_cost(cost)).run(
		kept_layout.compute_element_count(),
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
