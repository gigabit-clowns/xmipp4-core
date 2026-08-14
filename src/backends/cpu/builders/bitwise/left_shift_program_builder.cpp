// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/ops/bitwise/left_shift_operation.hpp>

#include <backends/cpu/builders/elementwise_program_builder.hpp>
#include <backends/cpu/builders/default_kernel_factory.hpp>
#include <backends/cpu/load_store.hpp>

namespace xmipp4
{
namespace cpu
{

namespace
{

struct left_shift_kernel
{
	// The operands promote to int before the operator sees them, so the
	// result is cast back to the element type explicitly rather than
	// narrowing on its way into store().
	template <typename T>
	void operator()(T *result, const T *value, const T *count) const noexcept
	{
		store(result, static_cast<T>(load(value) << load(count)));
	}
};

} // anonymous namespace

XMIPP4_REGISTER_ELEMENTWISE_PROGRAM_BUILDER(
	left_shift,
	ops::left_shift_operation,
	default_kernel_factory<left_shift_kernel>
);

} // namespace cpu
} // namespace xmipp4
