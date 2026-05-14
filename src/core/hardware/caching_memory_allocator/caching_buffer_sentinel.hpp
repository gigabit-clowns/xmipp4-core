// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/hardware/buffer_sentinel.hpp>

#include "../../config.hpp"

#include <xmipp4/core/span.hpp>

#include <vector>
#include <boost/container/flat_set.hpp>
#include <boost/container/small_vector.hpp>

namespace xmipp4 
{
namespace hardware
{

class memory_block;
class device_queue;
class caching_memory_allocator;

class caching_buffer_sentinel final
	: public buffer_sentinel
{
public: 
	caching_buffer_sentinel(
		caching_memory_allocator &allocator,
		memory_block &block
	);
	~caching_buffer_sentinel() override;

	span<device_queue *const> get_queues() const noexcept;

	void record_queue(device_queue &queue, bool exclusive) override;

private:
	using queue_set_type = boost::container::small_flat_set<
		device_queue*, 
		XMIPP4_SMALL_QUEUE_COUNT
	>;

	std::reference_wrapper<caching_memory_allocator> m_allocator;
	std::reference_wrapper<memory_block> m_block;
	queue_set_type m_queues;
};

} // namespace hardware
} // namespace xmipp4
