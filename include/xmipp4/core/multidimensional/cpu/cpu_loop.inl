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
template <typename... OutputTypes, typename... InputTypes>
inline
void cpu_loop<Op>::operator()(
	const std::tuple<OutputTypes*...> &output_pointers,
	const std::tuple<const InputTypes*...> &input_pointers,
	hardware::queue*
) const
{
	loop(
		std::tuple_cat(output_pointers, input_pointers),
		std::make_index_sequence<sizeof...(OutputTypes)+sizeof...(InputTypes)>()
	);
}

template <typename Op>
template <typename... Pointers, std::size_t... Is>
inline
void cpu_loop<Op>::loop(
	const std::tuple<Pointers...> &pointers,
	std::index_sequence<Is...>
) const
{
	array_iterator ite;

	if (!m_layout.iter(ite))
	{
		return;
	}

	const auto offsets = static_cast<const array_iterator&>(&).get_offsets();
	do
	{
		m_operation(std::get<Is>(pointers) + offsets[Is]...);
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
