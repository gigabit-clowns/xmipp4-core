// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/compute/memory_allocator.hpp>

namespace xmipp4 
{
namespace compute
{

class host_memory_allocator
    : public memory_allocator
{
public:
    memory_resource& get_memory_resource() const noexcept override;

    std::shared_ptr<buffer> allocate(
        std::size_t size, 
        std::size_t alignment, 
        device_queue *queue
    ) override;

};

} // namespace compute
} // namespace xmipp4
