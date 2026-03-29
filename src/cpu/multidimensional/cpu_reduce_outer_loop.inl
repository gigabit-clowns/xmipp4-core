// SPDX-License-Identifier: GPL-3.0-only

#include "cpu_reduce_outer_loop.hpp"

namespace xmipp4 
{
namespace multidimensional
{

template <typename OpInit, typename OpAcc>
inline
cpu_reduce_outer_loop<OpInit, OpAcc>::cpu_reduce_outer_loop(
	init_functor_type vector_init_handler,
	accum_functor_type vector_accum_handler,
	multi_array_access_layout access_layout
)
	: m_vector_init_handler(std::move(vector_init_handler))
	, m_vector_accum_handler(std::move(vector_accum_handler))
	, m_access_layout(std::move(access_layout))
	, m_first_reduction_axis(0)
{
	const auto strides = m_access_layout.get_strides(0);
	for (std::size_t i = 0; i < strides.size(); ++i)
	{
		if (strides[i] == 0)
		{
			m_first_reduction_axis = i;
			break;
		}
	}

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
	const std::size_t max_tile_width = 4096; // TODO determine

	multi_array_iterator ite;
	std::size_t tile_width;
	if (!(tile_width = m_access_layout.iter(ite)))
	{
		return;
	}

	// TODO initialize when necessary.
	// TODO decide optimal iteration pattern.

	const auto offsets = ite.get_offsets();
	do
	{
		tile_width = std::min(tile_width, max_tile_width)
		do
		{
			m_vector_accum_handler(std::get<Is>(pointers) + offsets[Is]..., count);
		} while (m_access_layout.next(ite, 1, m_first_reduction_axis));
	} while ((tile_width = m_access_layout.next(ite, tile_width, 0)));
}

template <typename OpInit, typename OpAcc>
inline
cpu_reduce_outer_loop<OpInit, OpAcc> make_cpu_reduce_outer_loop(
	OpInit vector_init_handler,
	OpAcc vector_accum_handler,
	multi_array_access_layout access_layout
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
