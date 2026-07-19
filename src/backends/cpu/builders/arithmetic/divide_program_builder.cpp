// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/ops/arithmetic/divide_operation.hpp>

#include <backends/cpu/builders/elementwise_program_builder.hpp>
#include <backends/cpu/builders/default_kernel_factory.hpp>
#include <backends/cpu/builders/type_dispatchers/homogeneous_type_dispatcher.hpp>
#include <backends/cpu/load_store.hpp>

namespace xmipp4
{
namespace cpu
{

namespace
{

struct divide_kernel
{
	template <typename T>
	void operator()(T *result, const T *x, const T *y) const noexcept
	{
		store(result, load(x) / load(y));
	}
};

template <typename T>
struct divide_predicate : std::integral_constant<
	bool,
	!std::is_same<T, bool>::value
> {};

} // anonymous namespace

XMIPP4_REGISTER_ELEMENTWISE_PROGRAM_BUILDER(
	divide,
	ops::divide_operation,
	default_kernel_factory<divide_kernel>,
	homogeneous_type_dispatcher<divide_predicate>
);

} // namespace cpu
} // namespace xmipp4
