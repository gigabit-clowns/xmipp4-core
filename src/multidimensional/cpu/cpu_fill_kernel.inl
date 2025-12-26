// SPDX-License-Identifier: GPL-3.0-only

#include "cpu_fill_kernel.hpp"

#include <xmipp4/core/hardware/buffer.hpp>
#include <xmipp4/core/hardware/device_queue.hpp>

namespace xmipp4 
{
namespace multidimensional
{

template <typename T, typename Q>
inline
cpu_fill_kernel<T, Q>::cpu_fill_kernel(
	array_access_layout access_layout,
	fill_value_type fill_value
) noexcept
	: m_access_layout(std::move(access_layout))
	, m_fill_value(fill_value)
{
}

template <typename T, typename Q>
inline
void cpu_fill_kernel<T, Q>::execute(
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
			"cpu_fill_kernel::execute: Expected no read-only operands."
		);
	}

	auto *destination_data = static_cast<output_value_type*>(
		read_write_operands[0]->get_host_ptr()
	);
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

	fill(destination_data);
}

template <typename T, typename Q>
inline
void cpu_fill_kernel<T, Q>::fill(output_value_type *destination) const
{
	// TODO
}

} // namespace multidimensional
} // namespace xmipp4
