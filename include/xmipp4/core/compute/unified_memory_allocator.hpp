// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "device_memory_allocator.hpp"
#include "host_memory_allocator.hpp"
#include "../platform/dynamic_shared_object.h"


namespace xmipp4 
{
namespace compute
{

class unified_buffer;

/**
 * @brief Interface merging the interfaces of device_memory_allocator and 
 * host_memory_allocator.
 * 
 * This interface unifies device_memory_allocator and host_memory_allocator 
 * interfaces. Although this interface does not explicitly participate in 
 * the compute API, it is useful to jointly implement the *_memory_allocator 
 * interfaces in unified memory architectures.
 * 
 */
class unified_memory_allocator
    : public device_memory_allocator
    , public host_memory_allocator
{
public:
    unified_memory_allocator() = default;
    unified_memory_allocator(const unified_memory_allocator &other) = default;
    unified_memory_allocator(unified_memory_allocator &&other) = default;
    ~unified_memory_allocator() override = default;

    unified_memory_allocator& 
    operator=(const unified_memory_allocator &other) = default;
    unified_memory_allocator& 
    operator=(unified_memory_allocator &&other) = default;

    virtual std::shared_ptr<unified_buffer> 
    create_unified_buffer(std::size_t size, std::size_t alignment) = 0;

    virtual std::shared_ptr<unified_buffer> 
    create_unified_buffer(std::size_t size, 
                          std::size_t alignment, 
                          device_queue &queue ) = 0;

    XMIPP4_CORE_API
    std::shared_ptr<device_buffer> 
    create_device_buffer(std::size_t size, 
                         std::size_t alignment,
                         device_queue &queue ) final;

    XMIPP4_CORE_API
    std::shared_ptr<host_buffer> 
    create_host_buffer(std::size_t size, 
                       std::size_t alignment,
                       device_queue &queue ) final;

    XMIPP4_CORE_API
    std::shared_ptr<host_buffer> 
    create_host_buffer(std::size_t size, std::size_t alignment) final;

}; 

} // namespace compute
} // namespace xmipp4
