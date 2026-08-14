// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/ops/bitwise/bitwise_or_operation.hpp>

#include <backends/cpu/builders/elementwise_program_builder.hpp>
#include <backends/cpu/builders/default_kernel_factory.hpp>
#include <backends/cpu/load_store.hpp>

namespace xmipp4
{
namespace cpu
{

namespace
{

struct bitwise_or_kernel
{
	// The operands promote to int before the operator sees them, so the
	// result is cast back to the element type explicitly rather than
	// narrowing on its way into store().
	template <typename T>
	void operator()(T *result, const T *x, const T *y) const noexcept
	{
		store(result, static_cast<T>(load(x) | load(y)));
	}
};

} // anonymous namespace

XMIPP4_REGISTER_ELEMENTWISE_PROGRAM_BUILDER(
	bitwise_or,
	ops::bitwise_or_operation,
	default_kernel_factory<bitwise_or_kernel>
);

} // namespace cpu
} // namespace xmipp4
