// SPDX-License-Identifier: GPL-3.0-only

#include "cpu_copy_kernel.hpp"

#include <xmipp4/core/hardware/buffer.hpp>
#include <xmipp4/core/hardware/device_queue.hpp>

namespace xmipp4 
{
namespace multidimensional
{
namespace
{

template <typename T, typename Q>
void copy(
	T *destination, 
	const Q *source, 
	std::ptrdiff_t destination_stride, 
	std::ptrdiff_t source_stride,
	std::size_t count
)
{
	std::ptrdiff_t destination_index = 0;
	std::ptrdiff_t source_index = 0;
	for (std::size_t i = 0; i < count; ++i)
	{
		destination[destination_index] = static_cast<T>(source[source_index]);
		destination_index += destination_stride;
		source_index += source_stride;
	}
}

template <typename T, typename Q>
void copy(
	T *destination, 
	const Q *source, 
	std::integral_constant<std::ptrdiff_t, 1>,
	std::integral_constant<std::ptrdiff_t, 1>,
	std::size_t count
)
{
	for (std::size_t i = 0; i < count; ++i)
	{
		destination[i] = static_cast<T>(source[i]);
	}
}

template <typename T>
void copy(
	T *destination, 
	const T *source, 
	std::integral_constant<std::ptrdiff_t, 1>,
	std::integral_constant<std::ptrdiff_t, 1>,
	std::size_t count
)
{
	std::memcpy(destination, source, count*sizeof(T));
}

} // anonymous namespace

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

	auto *destination_data = read_write_operands[0]->get_host_ptr();
	if (destination_data == nullptr)
	{
		throw std::invalid_argument(
			"cpu_copy_kernel::execute: Output operand is not "
			"host accessible."
		);
	}

	const auto *source_data = read_only_operands[0]->get_host_ptr();
	if (source_data == nullptr)
	{
		throw std::invalid_argument(
			"cpu_copy_kernel::execute: Input operand is not "
			"host accessible."
		);
	}

	if (source_data == destination_data)
	{
		// TODO in the future we may want a more sophisticated aliasing check.
		throw std::invalid_argument(
			"cpu_copy_kernel::execute: Input operand and output operand may "
			"alias one another"
		);
	}

	if (queue)
	{
		queue->wait_until_completed();
	}

	copy(
		static_cast<output_value_type*>(destination_data), 
		static_cast<const input_value_type*>(source_data)
	);
}

template <typename T, typename Q>
inline
void cpu_copy_kernel<T, Q>::copy(
	output_value_type *destination,
	const input_value_type *source
) const
{
	const auto destination_stride = m_output_stride;
	const auto source_stride = m_input_stride;

	array_iterator ite;
	std::size_t count;
	if (!(count = m_access_layout.iter(ite)))
	{
		return;
	}

	const auto offsets = ite.get_offsets();
	do
	{
		copy(
			destination + offsets[0],
			source + offsets[1],
			destination_stride,
			source_stride,
			count
		);
	}
	while((count = m_access_layout.next(ite, count)));
}

} // namespace multidimensional
} // namespace xmipp4
