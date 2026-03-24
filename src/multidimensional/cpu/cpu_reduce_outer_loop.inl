// SPDX-License-Identifier: GPL-3.0-only

#include "cpu_reduce_outer_loop.hpp"

namespace xmipp4 
{
namespace multidimensional
{

template <typename OpInit, typename OpAcc>
inline
cpu_reduce_outer_loop<OpInit, OpAcc>::cpu_reduce_outer_loop(
	init_operator_type vector_init_handler,
	accum_operator_type vector_accum_handler,
	array_access_layout access_layout
)
	: m_vector_init_handler(std::move(vector_init_handler))
	, m_vector_accum_handler(std::move(vector_accum_handler)
	, m_access_layout(std::move(access_layout))
{
}

template <typename OpInit, typename OpAcc>
template <typename... Pointers>
inline
void cpu_reduce_outer_loop<OpInit, OpAcc>::operator()(
	Pointers... pointers
) const
{
	loop_impl(
		std::make_tuple(pointers...),
		std::make_index_sequence<sizeof...(Pointers)>()
	);
}

template <typename OpInit, typename OpAcc>
template <typename... Pointers, std::size_t... Is>
inline
void cpu_reduce_outer_loop<OpInit, OpAcc>::loop_impl(
	const std::tuple<Pointers...> &pointers, 
	std::index_sequence<Is...>
) const
{
	// TODO initialize result.
	// TODO tiling for efficiency.
	array_iterator ite;
	std::size_t count;
	if (!(count = m_access_layout.iter(ite)))
	{
		return;
	}

	const auto offsets = ite.get_offsets();
	do
	{
		m_vector_accum_handler(std::get<Is>(pointers) + offsets[Is]..., count);
	} while ((count = m_access_layout.next(ite, count)));
}

template <typename OpInit, typename OpAcc>
inline
cpu_reduce_outer_loop<OpInit, OpAcc> make_cpu_reduce_outer_loop(
	OpInit vector_init_handler,
	OpAcc vector_accum_handler,
	array_access_layout access_layout
)
{
	return cpu_reduce_outer_loop<OpInit, OpAcc>(
		std::forward<OpInit>(vector_init_handler),
		std::forward<OpAcc>(vector_accum_handler),
		std::move(access_layout)
	);
}

} // namespace multidimensional
} // namespace xmipp4
