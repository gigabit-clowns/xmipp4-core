// SPDX-License-Identifier: GPL-3.0-only

#include "cpu_outer_loop.hpp"

namespace xmipp4 
{
namespace multidimensional
{

template <typename Op, typename... InnerStrides>
inline
cpu_outer_loop<Op, InnerStrides...>::cpu_outer_loop(
	operator_type vector_handler,
	array_access_layout access_layout,
	std::size_t inner_extent,
	const inner_strides_tuple_type &inner_strides
)
	: m_vector_handler(std::move(vector_handler))
	, m_access_layout(std::move(access_layout))
	, m_inner_extent(inner_extent)
	, m_inner_strides(inner_strides)
{
}

template <typename Op, typename... InnerStrides>
template <typename... Pointers>
inline
void cpu_outer_loop<Op, InnerStrides...>::operator()(
	Pointers... pointers
) const
{
	static_assert(
		sizeof...(InnerStrides) == sizeof...(Pointers),
		"Stride count must match the operand count"
	);
	loop_impl(
		std::make_tuple(pointers...),
		std::make_index_sequence<sizeof...(Pointers)>()
	);
}

template <typename Op, typename... InnerStrides>
template <typename... Pointers, std::size_t... Is>
inline
void cpu_outer_loop<Op, InnerStrides...>::loop_impl(
	const std::tuple<Pointers...> &pointers, 
	std::index_sequence<Is...>
) const
{
	array_iterator ite;
	if (!m_access_layout.iter_outer(ite))
	{
		return;
	}

	const auto offsets = ite.get_offsets();
	do
	{
		m_vector_handler(
			std::get<Is>(pointers) + offsets[Is]...,
			m_inner_extent,
			std::get<Is>(m_inner_strides)...
		);
	} while (m_access_layout.next(ite));
}

template <typename Op, typename... InnerStrides>
inline
cpu_outer_loop<Op, InnerStrides...> make_cpu_outer_loop(
	Op vector_handler,
	array_access_layout access_layout,
	std::size_t inner_extent,
	const std::tuple<InnerStrides...> &inner_strides
)
{
	return cpu_outer_loop<Op, InnerStrides...>(
		std::move(vector_handler),
		std::move(access_layout),
		inner_extent,
		inner_strides
	);
}

} // namespace multidimensional
} // namespace xmipp4
