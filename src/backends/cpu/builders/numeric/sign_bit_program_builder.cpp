// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/ops/numeric/sign_bit_operation.hpp>

#include <backends/cpu/builders/elementwise_program_builder.hpp>
#include <backends/cpu/builders/default_kernel_factory.hpp>
#include <backends/cpu/load_store.hpp>

#include <cmath>

namespace xmipp4
{
namespace cpu
{

namespace
{

struct sign_bit_kernel
{
	// Reports the bit rather than the ordering, so it holds for a
	// negative zero, which compares equal to a positive one, and for a
	// negatively signed not-a-number. That is the distinction from a
	// comparison against zero, and the reason the operation exists.
	template <typename T>
	void operator()(bool *result, const T *x) const noexcept
	{
		using std::signbit;
		store(result, signbit(load(x)));
	}
};

} // anonymous namespace

XMIPP4_REGISTER_ELEMENTWISE_PROGRAM_BUILDER(
	sign_bit,
	ops::sign_bit_operation,
	default_kernel_factory<sign_bit_kernel>
);

} // namespace cpu
} // namespace xmipp4
