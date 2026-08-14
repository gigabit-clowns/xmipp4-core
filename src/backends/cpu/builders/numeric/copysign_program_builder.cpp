// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/ops/numeric/copysign_operation.hpp>

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

struct copysign_kernel
{
	// The magnitude comes first and the sign second, as the operation
	// declares them.
	template <typename T>
	void operator()(
		T *result,
		const T *magnitude,
		const T *sign
	) const noexcept
	{
		using std::copysign;
		store(result, copysign(load(magnitude), load(sign)));
	}
};

} // anonymous namespace

XMIPP4_REGISTER_ELEMENTWISE_PROGRAM_BUILDER(
	copysign,
	ops::copysign_operation,
	default_kernel_factory<copysign_kernel>
);

} // namespace cpu
} // namespace xmipp4
