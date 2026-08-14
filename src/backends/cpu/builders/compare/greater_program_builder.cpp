// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/ops/compare/greater_operation.hpp>

#include <backends/cpu/builders/elementwise_program_builder.hpp>
#include <backends/cpu/builders/default_kernel_factory.hpp>
#include <backends/cpu/load_store.hpp>

namespace xmipp4
{
namespace cpu
{

namespace
{

struct greater_kernel
{
	// The result is a boolean whatever the operands are, so the element
	// type is deduced from the inputs alone. The rule already said so;
	// the builder reified it and handed this a bool to write into.
	template <typename T>
	void operator()(bool *result, const T *x, const T *y) const noexcept
	{
		store(result, load(x) > load(y));
	}
};

} // anonymous namespace

XMIPP4_REGISTER_ELEMENTWISE_PROGRAM_BUILDER(
	greater,
	ops::greater_operation,
	default_kernel_factory<greater_kernel>
);

} // namespace cpu
} // namespace xmipp4
