// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/ops/bitwise/bitwise_not_operation.hpp>

#include <backends/cpu/builders/elementwise_program_builder.hpp>
#include <backends/cpu/builders/default_kernel_factory.hpp>
#include <backends/cpu/load_store.hpp>

namespace xmipp4
{
namespace cpu
{

namespace
{

struct bitwise_not_kernel
{
	template <typename T>
	void operator()(T *result, const T *x) const noexcept
	{
		store(result, static_cast<T>(~load(x)));
	}

	// A boolean holds one bit of information but occupies eight, so
	// complementing its storage is not complementing its value: ~true
	// promotes to -2, which converts back to true and leaves the
	// operation doing nothing. Negating it is what NumPy does and the
	// only reading under which this is the bitwise complement of the
	// value rather than of its representation.
	void operator()(bool *result, const bool *x) const noexcept
	{
		store(result, !load(x));
	}
};

} // anonymous namespace

XMIPP4_REGISTER_ELEMENTWISE_PROGRAM_BUILDER(
	bitwise_not,
	ops::bitwise_not_operation,
	default_kernel_factory<bitwise_not_kernel>
);

} // namespace cpu
} // namespace xmipp4
