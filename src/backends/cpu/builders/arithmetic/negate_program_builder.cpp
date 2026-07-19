// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/ops/arithmetic/negate_operation.hpp>

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

struct negate_kernel
{
	template <typename T>
	void operator()(T *result, const T *x) const noexcept
	{
		store(result, -load(x));
	}
};

template <typename T>
struct negate_predicate : std::integral_constant<
	bool,
	(std::is_integral<T>::value && std::is_signed<T>::value) ||
	std::is_floating_point<T>::value ||
	std::is_same<T, float16_t>::value
> {};

} // anonymous namespace

XMIPP4_REGISTER_ELEMENTWISE_PROGRAM_BUILDER(
	negate,
	ops::negate_operation,
	default_kernel_factory<negate_kernel>,
	homogeneous_type_dispatcher<negate_predicate>
);

} // namespace cpu
} // namespace xmipp4
