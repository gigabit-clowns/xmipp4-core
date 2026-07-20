// SPDX-License-Identifier: GPL-3.0-only

#include "independent_type_dispatcher.hpp"

#include <xmipp4/core/numerical/numerical_type_dispatch.hpp>

#include <stdexcept>
#include <utility>

namespace xmipp4
{
namespace cpu
{

template <template <typename...> class Predicate>
template <typename F, std::size_t OutputCount, std::size_t InputCount>
std::shared_ptr<xmipp4::program>
independent_type_dispatcher<Predicate>::dispatch(
	F &&factory,
	const std::array<numerical_type, OutputCount> &output_types,
	const std::array<numerical_type, InputCount> &input_types
)
{
	return dispatch_output_types(
		std::forward<F>(factory),
		output_types,
		input_types,
		std::make_index_sequence<OutputCount>()
	);
}

template <template <typename...> class Predicate>
template <typename F, std::size_t InputCount, std::size_t... OutputIndices>
std::shared_ptr<xmipp4::program>
independent_type_dispatcher<Predicate>::dispatch_output_types(
	F &&factory,
	const std::array<numerical_type, sizeof...(OutputIndices)> &output_types,
	const std::array<numerical_type, InputCount> &input_types,
	std::index_sequence<OutputIndices...> /*output_indices*/
)
{
	return dispatch_numerical_types(
		[&factory, &input_types] (auto... tags)
			-> std::shared_ptr<xmipp4::program>
		{
			return dispatch_input_types(
				std::forward<F>(factory),
				type_list<typename decltype(tags)::type...>(),
				input_types,
				std::make_index_sequence<InputCount>()
			);
		},
		output_types[OutputIndices]...
	);
}

template <template <typename...> class Predicate>
template <typename F, typename... Outputs, std::size_t... InputIndices>
std::shared_ptr<xmipp4::program>
independent_type_dispatcher<Predicate>::dispatch_input_types(
	F &&factory,
	type_list<Outputs...> /*output_element_types*/,
	const std::array<numerical_type, sizeof...(InputIndices)> &input_types,
	std::index_sequence<InputIndices...> /*input_indices*/
)
{
	return dispatch_numerical_types(
		[&factory] (auto... tags) -> std::shared_ptr<xmipp4::program>
		{
			return invoke_if(
				std::integral_constant<
					bool,
					Predicate<
						Outputs...,
						typename decltype(tags)::type...
					>::value
				>(),
				std::forward<F>(factory),
				type_list<Outputs...>(),
				type_list<typename decltype(tags)::type...>()
			);
		},
		input_types[InputIndices]...
	);
}

template <template <typename...> class Predicate>
template <typename F, typename Outputs, typename Inputs>
std::shared_ptr<xmipp4::program>
independent_type_dispatcher<Predicate>::invoke_if(
	std::true_type /*supported*/,
	F &&factory,
	Outputs output_types,
	Inputs input_types
)
{
	return std::forward<F>(factory)(output_types, input_types);
}

template <template <typename...> class Predicate>
template <typename F, typename Outputs, typename Inputs>
std::shared_ptr<xmipp4::program>
independent_type_dispatcher<Predicate>::invoke_if(
	std::false_type /*supported*/,
	F&& /*factory*/,
	Outputs /*output_types*/,
	Inputs /*input_types*/
)
{
	throw std::invalid_argument(
		"independent_type_dispatcher: Unsupported data type combination."
	);
}

} // namespace cpu
} // namespace xmipp4
