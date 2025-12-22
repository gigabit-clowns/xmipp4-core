// SPDX-License-Identifier: GPL-3.0-only

#include "cpu_fill_kernel.hpp"

#include <xmipp4/core/hardware/buffer.hpp>
#include <xmipp4/core/hardware/device_queue.hpp>

namespace xmipp4 
{
namespace multidimensional
{

cpu_fill_kernel::cpu_fill_kernel(
	array_access_layout access_layout
) noexcept
	: m_access_layout(std::move(access_layout))
{
}

void cpu_fill_kernel::execute(
	span<const std::shared_ptr<hardware::buffer>> read_write_operands,
	span<const std::shared_ptr<const hardware::buffer>> read_only_operands,
	hardware::device_queue *queue
) const
{
	if (read_write_operands.size() != 1)
	{
		throw std::invalid_argument(
			"cpu_fill_kernel::execute: Expected exactly one "
			"read-write operand."
		);
	}
	if (read_write_operands[0] == nullptr)
	{
		throw std::invalid_argument(
			"cpu_fill_kernel::execute: Output operand is null."
		);
	}
	if (read_only_operands.size() != 0)
	{
		throw std::invalid_argument(
			"cpu_fill_kernel::execute: Expected no read-only operand."
		);
	}

	auto *destination_data = read_write_operands[0]->get_host_ptr();
	if (destination_data == nullptr)
	{
		throw std::invalid_argument(
			"cpu_fill_kernel::execute: Output operand is not "
			"host accessible."
		);
	}

	if (queue)
	{
		queue->wait_until_completed();
	}

	// TODO dispatch
}

} // namespace multidimensional
} // namespace xmipp4
