// SPDX-License-Identifier: GPL-3.0-only

#include "cpu_reduce_outer_loop.hpp"

#include <algorithm>

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
{
	const auto strides = m_access_layout.get_strides(0);
	for (std::size_t i = 0; i < strides.size(); ++i)
	{
		if (strides[i] == 0)
		{
			m_reduce_axes.push_back(i);
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
	std::size_t count;
	multi_array_iterator ite;
	if (!(count = m_access_layout.iter(ite)))
	{
		return;
	}

	const auto offsets = 
		static_cast<const multi_array_iterator&>(ite).get_offsets();
	const auto indices = 
		static_cast<const multi_array_iterator&>(ite).get_indices();
	while ((count = m_access_layout.next(ite, count)))
	{
		if (is_first_iter(indices))
		{
			m_vector_init_handler(
				std::get<Is>(pointers) + offsets[Is]..., 
				count
			);
		}
		else
		{
			m_vector_accum_handler(
				std::get<Is>(pointers) + offsets[Is]..., 
				count
			);
		}
	} 
}

template <typename OpInit, typename OpAcc>
bool cpu_reduce_outer_loop<OpInit, OpAcc>::is_first_iter(
	span<const std::size_t> indices
) const noexcept
{
	return std::all_of(
		m_reduce_axes.cbegin(),
		m_reduce_axes.cend(),
		[&indices] (std::size_t reduce_axis)
		{
			XMIPP4_ASSERT(reduce_axis < indices.size());
			return indices[reduce_axis] == 0;
		}
	)
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
