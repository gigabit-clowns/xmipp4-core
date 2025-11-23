// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/array_copy.hpp>

#include <xmipp4/core/multidimensional/array.hpp>
#include <xmipp4/core/hardware/memory_transfer_manager.hpp>
#include <xmipp4/core/hardware/memory_transfer.hpp>
#include <xmipp4/core/hardware/memory_allocator.hpp>
#include <xmipp4/core/hardware/buffer.hpp>
#include <xmipp4/core/hardware/copy_region.hpp>

#include <array>

namespace xmipp4 
{
namespace multidimensional
{

void copy(
	const array &src, 
	array &dst, 
	hardware::device_queue *queue
)
{
	// TODO
}

array transfer(
	const array &src, 
	hardware::memory_allocator &dst_allocator,
	hardware::memory_transfer_manager &transfer_manager,
	hardware::device_queue *queue,
	array *out
)
{
	auto result = array::empty(
		src.get_layout(),
		src.get_data_type(),
		dst_allocator,
		queue,
		out	
	);

	const auto *source_buffer = src.get_storage();
	if (!source_buffer)
	{
		throw std::invalid_argument(
			"Source array has no storage allocated."
		);
	}

	auto transfer = transfer_manager.create_transfer(
		src.get_storage()->get_memory_resource(),
		dst_allocator.get_memory_resource()
	);
	XMIPP4_ASSERT( transfer );

	auto *destination_buffer = result.get_storage();
	XMIPP4_ASSERT( destination_buffer );

	const std::array<hardware::copy_region, 1> regions = {
		hardware::copy_region{
			0,
			0,
			source_buffer->get_size()
		}
	};
	transfer->copy(
		*source_buffer,
		*destination_buffer,
		xmipp4::make_span(regions),
		queue
	);

	return result;
}

} // namespace multidimensional
} // namespace xmipp4

