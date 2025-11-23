// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/hardware/memory_allocator.hpp>

namespace xmipp4 
{
namespace hardware
{

/**
 * @brief Implementation of memory_allocator specialized for allocating memory
 * in host.
 * 
 * Unlike the caching allocator, this allocator specializes for host memory 
 * and does not cache memory blocks, as malloc is already highly optimized.
 * 
 */
class host_memory_allocator final
	: public memory_allocator
{
public:
	host_memory_allocator() = default;
	~host_memory_allocator() override = default;

	memory_resource& get_memory_resource() const noexcept override;

	std::size_t get_max_alignment() const noexcept override;

	std::shared_ptr<buffer> allocate(
		std::size_t size, 
		std::size_t alignment, 
		device_queue *queue
	) override;

};

} // namespace hardware
} // namespace xmipp4
