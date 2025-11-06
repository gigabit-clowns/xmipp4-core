// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/hardware/memory_sentinel.hpp>

#include "memory_block_pool.hpp"

#include <xmipp4/core/span.hpp>

#include <vector>

namespace xmipp4 
{
namespace hardware
{

class device_queue;
class caching_memory_allocator;

class caching_memory_sentinel final
    : public memory_sentinel
{
public: 
    caching_memory_sentinel(
        caching_memory_allocator &allocator,
        memory_block_pool::iterator block
    );
    caching_memory_sentinel(
        const caching_memory_sentinel &other
    ) = delete;
    caching_memory_sentinel(
        caching_memory_sentinel &&other
    ) = delete;
    ~caching_memory_sentinel() override;

    caching_memory_sentinel& operator=(
        const caching_memory_sentinel &other
    ) = delete;
    caching_memory_sentinel& operator=(
        caching_memory_sentinel &&other
    ) = delete;

    span<device_queue *const> get_queues() const noexcept;

    void record_queue(device_queue &queue, bool exclusive) override;


private:
    std::reference_wrapper<caching_memory_allocator> m_allocator;
    memory_block_pool::iterator m_block;
    std::vector<device_queue*> m_queues;

};

} // namespace hardware
} // namespace xmipp4
