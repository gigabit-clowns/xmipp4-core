// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/hardware/memory_allocator.hpp>

#include <xmipp4/core/hardware/memory_resource.hpp>

#include "memory_block_pool.hpp"
#include "memory_block_deferred_release.hpp"

namespace xmipp4 
{
namespace hardware
{

class caching_memory_allocator final
	: public memory_allocator
{
public:
	caching_memory_allocator(
		memory_resource &resource,
		std::size_t maximum_alignment,
		std::size_t heap_size_step    
	);
	~caching_memory_allocator() override;

	memory_resource& get_memory_resource() const noexcept override;

	std::size_t get_max_alignment() const noexcept override;

	std::shared_ptr<buffer> allocate(
		std::size_t size, 
		std::size_t alignment, 
		device_queue *queue
	) override;

	void recycle_block(
		memory_block &block, 
		span<device_queue *const> queues
	);

private:
	std::reference_wrapper<memory_resource> m_resource;
	device *m_device;
	memory_block_pool m_pool;
	memory_block_deferred_release m_deferred_release;
	std::size_t m_maximum_alignment;
	std::size_t m_heap_size_step;


	std::shared_ptr<buffer> create_buffer(memory_block &block);
};

} // namespace hardware
} // namespace xmipp4
