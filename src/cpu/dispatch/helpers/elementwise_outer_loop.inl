// SPDX-License-Identifier: GPL-3.0-only

#include "elementwise_outer_loop.hpp"

#include <tuple>
#include <utility>

namespace xmipp4
{
namespace dispatch
{
namespace detail
{

template <typename InnerLoop, typename... Pointers, std::size_t... Is>
inline
void run_elementwise_outer_loop_impl(
	InnerLoop &&inner_loop,
	const layout::access_layout &layout,
	const std::tuple<Pointers...> &pointers,
	std::index_sequence<Is...>
)
{
	layout::access_iterator ite;
	std::size_t count = layout.iter(ite);
	if (count == 0)
	{
		return;
	}

	const auto offsets = ite.get_offsets(); // Stable address
	do
	{
		inner_loop(std::get<Is>(pointers) + offsets[Is]..., count);
	}
	while ((count = layout.next(ite, count)));
}

} // namespace detail

template <typename InnerLoop, typename... Pointers>
inline
void run_elementwise_outer_loop(
	InnerLoop &&inner_loop,
	const layout::access_layout &layout,
	Pointers... pointers
)
{
	detail::run_elementwise_outer_loop_impl(
		std::forward<InnerLoop>(inner_loop),
		layout,
		std::make_tuple(pointers...),
		std::make_index_sequence<sizeof...(Pointers)>()
	);
}

} // namespace dispatch
} // namespace xmipp4
