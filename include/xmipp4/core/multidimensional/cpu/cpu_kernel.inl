// SPDX-License-Identifier: GPL-3.0-only

#include "cpu_kernel.hpp"

#include <tuple>

namespace xmipp4 
{
namespace multidimensional
{

template <typename Op, typename OutputPointerTuple, typename InputPointerTuple>
inline
cpu_kernel<Op, OutputPointerTuple, InputPointerTuple>::cpu_kernel(
	operation_type operation,
	array_access_layout access_layout
)
	: m_operation(std::move(operation))
	, m_access_layout(std::move(access_layout))
{
}

template <typename Op, typename OutputPointerTuple, typename InputPointerTuple>
inline
void cpu_kernel<Op, OutputPointerTuple, InputPointerTuple>::execute(
	span<const std::shared_ptr<hardware::buffer>> read_write_operands,
	span<const std::shared_ptr<const hardware::buffer>> read_only_operands,
	hardware::device_queue *queue
) const
{
	//const auto output_pointers = pack_output_pointers(read_write_operands);
	//const auto input_pointers = pack_input_pointers(read_only_operands);

	if (queue)
	{
		queue->wait_until_completed();
	}

	loop(output_pointers, input_pointers);
}

template <typename Op, typename OutputPointerTuple, typename InputPointerTuple>
inline
void cpu_kernel<Op, OutputPointerTuple, InputPointerTuple>::loop(
	const output_pointer_tuple_type &output_pointers,
	const input_pointer_tuple_type &input_pointers
)
{
	array_iterator ite;
	std::size_t count;
	if (!(count = m_access_layout.iter(ite)))
	{
		return;
	}

	const auto offsets = ite.get_offsets(); // Stable address
	do
	{
		m_operation(
			destination + offsets[0], // TODO apply offsets to input pointers

			count
		);
	}
	while((count = m_access_layout.next(ite, count)));
}

} // namespace multidimensional
} // namespace xmipp4
