// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/ops/reduction/argmax_operation.hpp>

#include <backends/cpu/builders/reduction_program_builder.hpp>
#include <backends/cpu/builders/default_kernel_factory.hpp>
#include <backends/cpu/builders/fold_reduction_kernel.hpp>
#include <backends/cpu/load_store.hpp>

#include <xmipp4/core/meta/type_list.hpp>

#include <cstddef>
#include <cstdint>

namespace xmipp4
{
namespace cpu
{

namespace
{

/**
 * @brief Locate the largest element along the reduced axes.
 *
 * Two accumulators rather than one pair: the running extremum and the place
 * it was found are kept in separate tiles, so the comparison across the
 * outputs of a tile stays a contiguous walk of each rather than a stride
 * over interleaved fields.
 *
 * The extremum accumulates in the computation type of the operand, while the
 * answer is the index the operation's rule fixes, so neither accumulator has
 * the type of the output. Only the index is written out; the value it was
 * found at is dropped.
 *
 * There is no identity. A largest element of nothing does not exist, and the
 * loop rejects that case rather than inventing one, as NumPy does.
 */
struct argmax_kernel
{
	template <typename Outputs, typename Inputs>
	struct accumulators
	{
		using type = type_list<
			typename reduction_compute_type<
				typename type_list_element<0, Inputs>::type
			>::type,
			std::int64_t
		>;
	};

	template <typename Accumulator, typename T>
	void seed(
		Accumulator &best,
		std::int64_t &where,
		const T *value,
		std::size_t position
	) const noexcept
	{
		best = load(value);
		where = static_cast<std::int64_t>(position);
	}

	template <typename Accumulator, typename T>
	void combine(
		Accumulator &best,
		std::int64_t &where,
		const T *value,
		std::size_t position
	) const noexcept
	{
		// Strictly greater, so that a tie keeps the first place it was seen.
		const auto current = static_cast<Accumulator>(load(value));
		if (current > best)
		{
			best = current;
			where = static_cast<std::int64_t>(position);
		}
	}

	template <typename Accumulator>
	void merge(
		Accumulator &best,
		std::int64_t &where,
		const Accumulator &other_best,
		const std::int64_t &other_where
	) const noexcept
	{
		if (other_best > best)
		{
			best = other_best;
			where = other_where;
		}
	}

	void merge(
		bool &best,
		std::int64_t &where,
		const bool &other_best,
		const std::int64_t &other_where
	) const noexcept
	{
		if (other_best && !best)
		{
			best = other_best;
			where = other_where;
		}
	}

	template <typename U, typename Accumulator>
	void finalize(
		U *result,
		const Accumulator &best,
		const std::int64_t &where,
		std::size_t /*count*/
	) const noexcept
	{
		static_cast<void>(best);
		store(result, where);
	}
};

} // anonymous namespace

XMIPP4_REGISTER_INDEXED_REDUCTION_PROGRAM_BUILDER(
	argmax,
	ops::argmax_operation,
	default_kernel_factory<argmax_kernel>
);

} // namespace cpu
} // namespace xmipp4
