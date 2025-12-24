// SPDX-License-Identifier: GPL-3.0-only

#include "cpu_copy_kernel.hpp"

#include <xmipp4/core/hardware/buffer.hpp>
#include <xmipp4/core/hardware/device_queue.hpp>

namespace xmipp4 
{
namespace multidimensional
{

template <typename T, typename Q>
inline
cpu_copy_kernel<T, Q>::cpu_copy_kernel(
	array_access_layout access_layout
) noexcept
	: m_access_layout(std::move(access_layout))
{
}

template <typename T, typename Q>
inline
void cpu_copy_kernel<T, Q>::execute(
	span<const std::shared_ptr<hardware::buffer>> read_write_operands,
	span<const std::shared_ptr<const hardware::buffer>> read_only_operands,
	hardware::device_queue *queue
) const
{
	if (read_write_operands.size() != 1)
	{
		throw std::invalid_argument(
			"cpu_copy_kernel::execute: Expected exactly one "
			"read-write operand."
		);
	}
	if (read_write_operands[0] == nullptr)
	{
		throw std::invalid_argument(
			"cpu_copy_kernel::execute: Output operand is null."
		);
	}
	if (read_only_operands.size() != 1)
	{
		throw std::invalid_argument(
			"cpu_copy_kernel::execute: Expected exactly one "
			"read-only operand."
		);
	}
	if (read_only_operands[0] == nullptr)
	{
		throw std::invalid_argument(
			"cpu_copy_kernel::execute: Input operand is null."
		);
	}

	auto *destination_data = static_cast<output_value_type*>(
		read_write_operands[0]->get_host_ptr()
	);
	if (destination_data == nullptr)
	{
		throw std::invalid_argument(
			"cpu_copy_kernel::execute: Output operand is not "
			"host accessible."
		);
	}

	const auto *source_data = static_cast<const input_value_type*>(
		read_only_operands[0]->get_host_ptr()
	);
	if (source_data == nullptr)
	{
		throw std::invalid_argument(
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
