// SPDX-License-Identifier: GPL-3.0-only

#include "elementwise_loop.hpp"

#include "inner_loop_stride_dispatch.hpp"
#include "elementwise_outer_loop.hpp"

#include <tuple>
#include <utility>
#include <type_traits>

namespace xmipp4
{
namespace cpu
{
namespace detail
{

template <
	typename Kernel,
	typename... StrideTags,
	std::size_t... Is,
	typename... LoopArgs
>
inline
void invoke_elementwise_kernel(
	const Kernel &kernel,
	const std::tuple<StrideTags...> &stride_tags,
	std::index_sequence<Is...>,
	LoopArgs... loop_args
)
{
	// loop_args holds the per-vector pointers followed by the element count;
	// the resolved stride tags are appended after them.
	kernel(
		loop_args...,
		std::get<Is>(stride_tags)...
	);
}

} // namespace detail

template <typename Kernel, typename... Pointers>
inline
void run_elementwise_loop(
	const Kernel &kernel,
	const joint_layout &layout,
	Pointers... pointers
)
{
	dispatch_inner_loop_strides(
		[&kernel, &layout, &pointers...] (auto stride_tags)
		{
			run_elementwise_outer_loop(
				[&kernel, &stride_tags]
				(Pointers... vector_pointers, std::size_t count)
				{
					detail::invoke_elementwise_kernel(
						kernel,
						stride_tags,
						std::make_index_sequence<sizeof...(Pointers)>(),
						vector_pointers...,
						count
					);
				},
				layout,
				pointers...
			);
		},
		layout,
		std::integral_constant<std::size_t, sizeof...(Pointers)>()
	);
}

} // namespace cpu
} // namespace xmipp4
