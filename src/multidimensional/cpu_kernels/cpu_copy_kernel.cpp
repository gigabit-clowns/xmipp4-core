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

template <typename Dst, typename Src>
void copy_data_loop(
	Dst *dst_data,
	const Src *src_data,
	const std::size_t *extents,
	const std::ptrdiff_t *dst_strides,
	const std::ptrdiff_t *src_strides,
	std::size_t ndim
)
{
	if (ndim == 0)
	{
		*dst_data = *src_data;
	}
	else if (ndim == 1)
	{
		for (std::size_t i = 0; i < extents[0]; ++i)
		{
			dst_data[i * dst_strides[0]] = src_data[i * src_strides[0]];
		}
	}
	else
	{
		for (std::size_t i = 0; i < extents[0]; ++i)
		{
			copy_data_loop(
				dst_data + i * dst_strides[0],
				src_data + i * src_strides[0],
				extents + 1,
				dst_strides + 1,
				src_strides + 1,
				ndim - 1
			);
		}
	}
}

template <typename Dst, typename Src>
void copy_data(
	Dst *dst_data,
	const Src *src_data,
	const array_access_layout &layout
)
{
	const auto extents = layout.get_extents();
	const auto dst_strides = layout.get_strides(0);
	const auto src_strides = layout.get_strides(1);
	const auto dst_offset = layout.get_offset(0);
	const auto src_offset = layout.get_offset(1);

	const std::size_t ndim = extents.size();
	XMIPP4_ASSERT(dst_strides.size() == ndim);
	XMIPP4_ASSERT(src_strides.size() == ndim);

	copy_data_loop(
		dst_data + dst_offset,
		src_data + src_offset,
		extents.data(),
		dst_strides.data(),
		src_strides.data(),
		ndim
	);
}

} // anonymous namespace

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

	const auto *source_data = read_only_operands[0]->get_host_ptr();
	if (source_data == nullptr)
	{
		throw std::runtime_error(
			"cpu_copy_kernel::execute: Input operand is not "
			"host accessible."
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

	if (queue)
	{
		queue->wait_until_completed();
	}

	const auto &access_layout = m_access_layout;
	visit(
		[&access_layout, destination_data, source_data] 
		(auto dst_type, auto src_type) {
			using Dst = decltype(dst_type);
			using Src = decltype(src_type);
			//copy_data<Dst, Src>(
			//	static_cast<Dst*>(destination_data),
			//	static_cast<const Src*>(source_data),
			//	access_layout
			//);
		},
		m_output_data_type,
		m_input_data_type
	);
}

} // namespace multidimensional
} // namespace xmipp4
