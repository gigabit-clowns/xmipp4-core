// SPDX-License-Identifier: GPL-3.0-only

#include "type_and_inner_stride_dispatch.hpp"

#include "inner_loop_stride_dispatch.hpp"

#include <xmipp4/core/numerical/numerical_type_dispatch.hpp>
#include <xmipp4/core/meta/type_wrap.hpp>

#include <tuple>
#include <type_traits>
#include <utility>

namespace xmipp4
{

template <std::size_t NOperands, typename F, typename... DataTypes>
auto dispatch_types_and_inner_strides(
	F &&callable,
	access_layout access_layout,
	DataTypes... data_types
)
{
	return dispatch_numerical_types(
		[&callable, &access_layout] (auto... type_tags)
		{
			type_list<typename decltype(type_tags)::type...> types;
			return dispatch_inner_loop_strides(
				[&callable, &access_layout, types] (auto inner_strides)
				{
					return callable(
						std::move(access_layout),
						types,
						inner_strides
					);
				},
				access_layout,
				std::integral_constant<std::size_t, NOperands>()
			);
		},
		native_type_map(),
		data_types...
	);
}

} // namespace xmipp4
