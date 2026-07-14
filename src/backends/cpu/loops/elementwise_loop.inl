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

template <typename Kernel, typename... Pointers>
inline
void run_elementwise_loop(
	const Kernel &kernel,
	const joint_layout &layout,
	Pointers... pointers
)
{
	dispatch_inner_loop_strides(
		[&kernel, &layout, &pointers...] (auto strides)
		{
			run_elementwise_outer_loop(
				[&kernel, &strides]
				(Pointers... vector_pointers, std::size_t count)
				{
					kernel(
						std::make_tuple(vector_pointers...),
						strides,
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
