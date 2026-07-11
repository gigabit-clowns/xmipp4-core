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
namespace cpu
{

template <std::size_t NOperands, typename F, typename... DataTypes>
auto dispatch_types_and_inner_strides(
	F &&callable,
	joint_layout layout,
	DataTypes... data_types
)
{
	return dispatch_numerical_types(
		[&callable, &layout] (auto... type_tags)
		{
			type_list<typename decltype(type_tags)::type...> types;
			return dispatch_inner_loop_strides(
				[&callable, &layout, types] (auto inner_strides)
				{
					return callable(
						std::move(layout),
						types,
						inner_strides
					);
				},
				layout,
				std::integral_constant<std::size_t, NOperands>()
			);
		},
		native_type_map(),
		data_types...
	);
}

} // namespace cpu
} // namespace xmipp4
