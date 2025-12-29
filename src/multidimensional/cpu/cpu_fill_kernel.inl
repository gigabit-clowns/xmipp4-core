// SPDX-License-Identifier: GPL-3.0-only

#include "cpu_fill_kernel.hpp"

#include <xmipp4/core/hardware/buffer.hpp>
#include <xmipp4/core/hardware/device_queue.hpp>

#include <algorithm>

namespace xmipp4 
{
namespace multidimensional
{
namespace
{

template <typename T>
void fill_impl(
	T *destination, 
	const T &fill_value,
	std::ptrdiff_t destination_stride, 
	std::size_t count
)
{
	std::ptrdiff_t destination_index = 0;
	for (std::size_t i = 0; i < count; ++i)
	{
		destination[destination_index] = fill_value;
		destination_index += destination_stride;
	}
}

template <typename T>
void fill_impl(
	T *destination, 
	const T &fill_value,
	std::integral_constant<std::ptrdiff_t, 1>,
	std::size_t count
)
{
	std::fill_n(destination, count, fill_value);
}

} // anonymous namespace

template <typename T>
inline
cpu_fill_kernel<T>::cpu_fill_kernel(
	array_access_layout access_layout,
	output_value_type fill_value
) noexcept
	: m_access_layout(std::move(access_layout))
	, m_fill_value(fill_value)
{
	if (m_access_layout.get_extents().empty())
	{
		m_output_stride = 1;
	}
	else
	{
		m_output_stride = m_access_layout.get_strides(0).front();
	}
}

template <typename T>
inline
void cpu_fill_kernel<T>::execute(
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

	fill(static_cast<output_value_type*>(destination_data));
}

template <typename T>
inline
void cpu_fill_kernel<T>::fill(output_value_type *destination) const
{
	const auto destination_stride = m_output_stride;

	array_iterator ite;
	std::size_t count;
	if (!(count = m_access_layout.iter(ite)))
	{
		return;
	}

	const auto offsets = ite.get_offsets();
	do
	{
		fill_impl(
			destination + offsets[0],
			m_fill_value,
			destination_stride,
			count
		);
	}
	while((count = m_access_layout.next(ite, count)));
}

} // namespace multidimensional
} // namespace xmipp4
