// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/ops/selection/clip_operation.hpp>

#include <backends/cpu/builders/elementwise_program_builder.hpp>
#include <backends/cpu/builders/default_kernel_factory.hpp>
#include <backends/cpu/kernels/ordering.hpp>
#include <backends/cpu/load_store.hpp>

namespace xmipp4
{
namespace cpu
{

namespace
{

struct clip_kernel
{
	// Composed of the two halves it is made of, so bounds that cross
	// leave the upper one winning, as NumPy has it, and an unordered
	// value propagates through both.
	template <typename T>
	void operator()(
		T *result,
		const T *value,
		const T *lower,
		const T *upper
	) const noexcept
	{
		const auto bounded_below = maximum_of(load(value), load(lower));
		store(result, minimum_of(bounded_below, load(upper)));
	}
};

} // anonymous namespace

XMIPP4_REGISTER_ELEMENTWISE_PROGRAM_BUILDER(
	clip,
	ops::clip_operation,
	default_kernel_factory<clip_kernel>
);

} // namespace cpu
} // namespace xmipp4
