// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/hardware/buffer_sentinel.hpp>

#include "memory_block_pool.hpp"
#include "../../config.hpp"

#include <xmipp4/core/span.hpp>

#include <vector>
#include <boost/container/flat_set.hpp>
#include <boost/container/small_vector.hpp>

namespace xmipp4 
{
namespace hardware
{

class device_queue;
class caching_memory_allocator;

class caching_buffer_sentinel final
    : public buffer_sentinel
{
public: 
    caching_buffer_sentinel(
        caching_memory_allocator &allocator,
        memory_block_pool::iterator block
    );
    caching_buffer_sentinel(
        const caching_buffer_sentinel &other
    ) = delete;
    caching_buffer_sentinel(
        caching_buffer_sentinel &&other
    ) = delete;
    ~caching_buffer_sentinel() override;

    caching_buffer_sentinel& operator=(
        const caching_buffer_sentinel &other
    ) = delete;
    caching_buffer_sentinel& operator=(
        caching_buffer_sentinel &&other
    ) = delete;

    span<device_queue *const> get_queues() const noexcept;

    void record_queue(device_queue &queue, bool exclusive) override;

private:
    using queue_set_type = boost::container::small_flat_set<
        device_queue*, 
        XMIPP4_SMALL_QUEUE_COUNT
    >;

    std::reference_wrapper<caching_memory_allocator> m_allocator;
    memory_block_pool::iterator m_block;
    queue_set_type m_queues;

};

} // namespace hardware
} // namespace xmipp4
