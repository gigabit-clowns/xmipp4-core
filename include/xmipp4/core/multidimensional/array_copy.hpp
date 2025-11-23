// SPDX-License-Identifier: GPL-3.0-only

#pragma once

namespace xmipp4 
{
namespace hardware
{

class device_queue;
class memory_allocator;
class memory_transfer_manger;

} // namespace hardware

namespace multidimensional
{

class array;

void copy(
	const array &src, 
	array &dst, 
	hardware::device_queue *queue
);

array transfer(
	const array &src, 
	hardware::memory_allocator &dst_allocator,
	hardware::memory_transfer_manager &transfer_manager,
	hardware::device_queue *queue,
	array *out
);

} // namespace multidimensional
} // namespace xmipp4
