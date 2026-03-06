// SPDX-License-Identifier: GPL-3.0-only

#include "cpu_inner_loop_dispatch.hpp"

namespace xmipp4 
{

namespace multidimensional
{

namespace detail
{
	
template <typename F>
XMIPP4_INLINE_CONSTEXPR
auto dispatch_single_stride(F&& visitor, std::ptrdiff_t stride) 
{
	XMIPP4_CONST_CONSTEXPR std::integral_constant<std::ptrdiff_t, 0> zero;
	XMIPP4_CONST_CONSTEXPR std::integral_constant<std::ptrdiff_t, 1> one;

    switch (stride) 
	{
	case 0: 
		return std::forward<F>(visitor)(zero);
	case 1: 
		return std::forward<F>(visitor)(one);
	default: 
		return std::forward<F>(visitor)(stride);
    }
}

template <typename F>
XMIPP4_INLINE_CONSTEXPR
auto dispatch_strides(F&& visitor) 
{
    return std::forward<F>(visitor)();
}

template <typename F, typename... Integers>
XMIPP4_INLINE_CONSTEXPR
auto dispatch_strides(
	F&& visitor, 
	std::ptrdiff_t stride, 
	Integers ...other_strides
) 
{
    return dispatch_single_stride(
		[&visitor, &other_strides...](auto tag) 
		{
			const auto bound_visitor = [tag, &visitor](auto... other_tags) 
			{
   				return std::forward<F>(visitor)(tag, other_tags...);
        	};

        	return dispatch_strides(
				bound_visitor,
				other_strides...
			);
   		},
		stride
	);
}

} // namespace detail

template <typename F, std::size_t N>
inline
auto dispatch_inner_loop(
	F &&callable, 
	const array_access_layout &layout, 
	std::integral_constant<std::size_t, N>
)
{
	return dispatch_inner_loop(
		std::forward<F>(callable), 
		layout,
		std::make_index_sequence<N>()
	);
}

template <typename F, std::size_t... Is>
inline
auto dispatch_inner_loop(
	F &&callable, 
	const array_access_layout &layout, 
	std::index_sequence<Is...>
)
{
	const auto extents = layout.get_extents();
	if (extents.empty())
	{
		// Zero dimensions, no stride to be obtained. Invoke the function
		// with one element.
		XMIPP4_CONST_CONSTEXPR std::integral_constant<std::ptrdiff_t, 0> zero;
		return std::forward<F>(callable)(
			1, 
			std::make_tuple(((void)Is, zero)...)
		);
	}

	XMIPP4_CONST_CONSTEXPR std::size_t inner_index = 0;
	const auto inner_extent = extents[inner_index];
	return detail::dispatch_strides(
		[&callable, inner_extent] (auto... stride_tags)
		{
			return std::forward<F>(callable)(
				inner_extent,
				std::make_tuple(stride_tags...)
			);
		},
		(layout.get_strides(Is)[inner_index])...
	);
}

} // namespace multidimensional
} // namespace xmipp4
