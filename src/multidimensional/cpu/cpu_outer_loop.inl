// SPDX-License-Identifier: GPL-3.0-only

#include "cpu_outer_loop.hpp"

namespace xmipp4 
{
namespace multidimensional
{

template <typename Op>
inline
cpu_outer_loop<Op>::cpu_outer_loop(
	operator_type vector_handler,
	array_access_layout access_layout
)
	: m_vector_handler(std::move(vector_handler))
	, m_access_layout(std::move(access_layout))
{
}

template <typename Op>
template <typename... Pointers>
inline
void cpu_outer_loop<Op>::operator()(
	Pointers... pointers
) const
{
	loop_impl(
		std::make_tuple(pointers...),
		std::make_index_sequence<sizeof...(Pointers)>()
	);
}

template <typename Op>
template <typename... Pointers, std::size_t... Is>
inline
void cpu_outer_loop<Op>::loop_impl(
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
		m_vector_handler(std::get<Is>(pointers) + offsets[Is]...);
	} while (m_access_layout.next(ite));
}

template <typename Op>
inline
cpu_outer_loop<Op> make_cpu_outer_loop(
	Op vector_handler,
	array_access_layout access_layout
)
{
	return cpu_outer_loop<Op>(
		std::move(vector_handler),
		std::move(access_layout)
	);
}

} // namespace multidimensional
} // namespace xmipp4
