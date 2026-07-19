// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/ops/assignment/copy_operation.hpp>

#include <backends/cpu/builders/elementwise_program_builder.hpp>
#include <backends/cpu/builders/default_kernel_factory.hpp>
#include <backends/cpu/builders/type_dispatchers/independent_type_dispatcher.hpp>
#include <backends/cpu/load_store.hpp>

namespace xmipp4
{
namespace cpu
{

namespace
{

struct copy_kernel
{
	template <typename T, typename Q>
	void operator()(T *result, const Q *x) const noexcept
	{
		cast(result, x);
	}
};

template <typename T, typename Q>
struct copy_predicate : std::is_convertible<Q, T> {};

} // anonymous namespace

XMIPP4_REGISTER_ELEMENTWISE_PROGRAM_BUILDER(
	copy,
	ops::copy_operation,
	default_kernel_factory<copy_kernel>,
	independent_type_dispatcher<copy_predicate>
);

} // namespace cpu
} // namespace xmipp4
