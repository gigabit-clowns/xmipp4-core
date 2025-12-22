// SPDX-License-Identifier: GPL-3.0-only

#include "cpu_copy_kernel.hpp"

#include <xmipp4/core/hardware/buffer.hpp>
#include <xmipp4/core/hardware/device_queue.hpp>

namespace xmipp4 
{
namespace multidimensional
{

cpu_copy_kernel::cpu_copy_kernel(
	array_access_layout access_layout,
	numerical_type output_data_type,
	numerical_type input_data_type
) noexcept
	: m_access_layout(std::move(access_layout))
	, m_output_data_type(output_data_type)
	, m_input_data_type(input_data_type)
{
}

void cpu_copy_kernel::execute(
	span<const std::shared_ptr<hardware::buffer>> read_write_operands,
	span<const std::shared_ptr<const hardware::buffer>> read_only_operands,
	hardware::device_queue *queue
) const
{
	if (read_write_operands.size() != 1)
	{
		throw std::runtime_error(
			"cpu_copy_kernel::execute: Expected exactly one "
			"read-write operand."
		);
	}
	if (read_write_operands[0] == nullptr)
	{
		throw std::runtime_error(
			"cpu_copy_kernel::execute: Output operand is null."
		);
	}
	if (read_only_operands.size() != 1)
	{
		throw std::runtime_error(
			"cpu_copy_kernel::execute: Expected exactly one "
			"read-only operand."
		);
	}
	if (read_only_operands[0] == nullptr)
	{
		throw std::runtime_error(
			"cpu_copy_kernel::execute: Input operand is null."
		);
	}

	auto *destination_data = read_write_operands[0]->get_host_ptr();
	if (destination_data == nullptr)
	{
		throw std::runtime_error(
			"cpu_copy_kernel::execute: Output operand is not "
			"host accessible."
		);
	}

	const auto *source_data = read_only_operands[0]->get_host_ptr();
	if (source_data == nullptr)
	{
		throw std::runtime_error(
			"cpu_copy_kernel::execute: Input operand is not "
			"host accessible."
		);
	}

	if (queue)
	{
		queue->wait_until_completed();
	}

	// TODO
}

} // namespace multidimensional
} // namespace xmipp4
