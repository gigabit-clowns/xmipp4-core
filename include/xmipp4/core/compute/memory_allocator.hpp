// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <memory>

#include "../platform/dynamic_shared_object.h"

namespace xmipp4 
{
namespace compute
{

class buffer;
class device_queue;



class XMIPP4_CORE_API memory_allocator
{
public:
    memory_allocator() = default;
    memory_allocator(const memory_allocator &other) = default;
    memory_allocator(memory_allocator &&other) = default;
    virtual ~memory_allocator() = default;

    memory_allocator& operator=(const memory_allocator &other) = default;
    memory_allocator& operator=(memory_allocator &&other) = default;

    virtual
    std::shared_ptr<buffer> allocate(
        std::size_t size, 
        std::size_t alignment, 
        device_queue *queue
    ) = 0;

}; 

} // namespace compute
} // namespace xmipp4
