// SPDX-License-Identifier: GPL-3.0-only

#include "cpu_loop.hpp"

namespace xmipp4 
{
namespace multidimensional
{

template <typename Op>
inline
cpu_loop<Op>::cpu_loop(
	array_access_layout layout,
	operation_type operation
)
	: m_layout(std::move(layout))
	, m_operation(std::move(operation))
{
}

template <typename Op>
template <typename... Types>
inline
void cpu_loop<Op>::operator()(
	const std::tuple<Types*...> &operand_pointers,
	hardware::queue* queue
) const
{
	if (queue)
	{
		queue->wait_for_completion();
	}

	loop(operand_pointers, std::make_index_sequence<sizeof...(Types)>());
}

template <typename Op>
template <typename... Pointers, std::size_t... Is>
inline
void cpu_loop<Op>::loop(
	const std::tuple<Pointers...> &operand_pointers,
	std::index_sequence<Is...>
) const
{
	array_iterator ite;
	if (!m_layout.iter_outer(ite))
	{
		return;
	}

	const auto offsets = static_cast<const array_iterator&>(ite).get_offsets();
	do
	{
		m_operation(std::get<Is>(operand_pointers) + offsets[Is]...);
	} while (m_layout.next(ite));
}

template<typename Op>
inline
cpu_loop<Op> make_cpu_loop(array_access_layout layout, Op operation)
{
	return cpu_loop<Op>(std::move(layout), std::move(operation));
}

} // namespace multidimensional
} // namespace xmipp4
