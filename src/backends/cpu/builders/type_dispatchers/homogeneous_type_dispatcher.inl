// SPDX-License-Identifier: GPL-3.0-only

#include "homogeneous_type_dispatcher.hpp"

#include <xmipp4/core/numerical/numerical_type_dispatch.hpp>
#include <xmipp4/core/meta/type_list.hpp>

#include <sstream>
#include <stdexcept>
#include <utility>

namespace xmipp4
{
namespace cpu
{

template <template <typename> class Predicate>
template <typename F, std::size_t OutputCount, std::size_t InputCount>
std::shared_ptr<xmipp4::program>
homogeneous_type_dispatcher<Predicate>::dispatch(
	F &&factory,
	const std::array<numerical_type, OutputCount> &output_types,
	const std::array<numerical_type, InputCount> &input_types
)
{
	const numerical_type reference = output_types.front();
	for (std::size_t i = 1; i < output_types.size(); ++i)
	{
		if (output_types[i] != reference)
		{
			std::ostringstream oss;
			oss << "homogeneous_type_dispatcher: Mismatching data type "
				<< "encountered at output index " << i;
			throw std::invalid_argument(oss.str());
		}
	}

	for (std::size_t i = 0; i < input_types.size(); ++i)
	{
		if (input_types[i] != reference)
		{
			std::ostringstream oss;
			oss << "homogeneous_type_dispatcher: Mismatching data type "
				<< "encountered at input index " << i;
			throw std::invalid_argument(oss.str());
		}
	}

	return dispatch_numerical_types(
		[&factory] (auto tag) -> std::shared_ptr<xmipp4::program>
		{
			using type = typename decltype(tag)::type;
			return invoke_if(
				std::integral_constant<bool, Predicate<type>::value>(),
				std::forward<F>(factory),
				typename repeated_type_list<type, OutputCount>::type(),
				typename repeated_type_list<type, InputCount>::type()
			);
		},
		reference
	);
}

template <template <typename> class Predicate>
template <typename F, typename Outputs, typename Inputs>
std::shared_ptr<xmipp4::program>
homogeneous_type_dispatcher<Predicate>::invoke_if(
	std::true_type /*supported*/,
	F &&factory,
	Outputs output_types,
	Inputs input_types
)
{
	return std::forward<F>(factory)(output_types, input_types);
}

template <template <typename> class Predicate>
template <typename F, typename Outputs, typename Inputs>
std::shared_ptr<xmipp4::program>
homogeneous_type_dispatcher<Predicate>::invoke_if(
	std::false_type /*supported*/,
	F&& /*factory*/,
	Outputs /*output_types*/,
	Inputs /*input_types*/
)
{
	throw std::invalid_argument(
		"homogeneous_type_dispatcher: Unsupported data type."
	);
}

} // namespace cpu
} // namespace xmipp4
