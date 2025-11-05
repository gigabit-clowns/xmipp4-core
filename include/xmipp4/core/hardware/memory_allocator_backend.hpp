// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <memory>

#include "../backend_priority.hpp"
#include "../platform/dynamic_shared_object.h"

namespace xmipp4 
{
namespace hardware
{

class memory_allocator;
class memory_resource;

/**
 * @brief Abstract class that represents a memory allocator backend.
 * 
 */
class XMIPP4_CORE_API memory_allocator_backend
{
public:
    memory_allocator_backend() = default;
    memory_allocator_backend(const memory_allocator_backend &other) = default;
    memory_allocator_backend(memory_allocator_backend &&other) = default;
    virtual ~memory_allocator_backend() = default;

    memory_allocator_backend& 
    operator=(const memory_allocator_backend &other) = default;
    memory_allocator_backend& 
    operator=(memory_allocator_backend &&other) = default;

    /**
     * @brief Get the priority of this backend for the provided memory_resource.
     * 
     * @param resource The memory resource to be checked.
     * @return backend_priority The priority.
     */
    virtual backend_priority get_priority(
        memory_resource &resource
    ) const noexcept = 0;

    /**
     * @brief Create a memory allocator to allocate buffers on the provided
     * memory_resource.
     * 
     * @param resource The memory resource where the allocator creates memory
     * buffers.
     * @return std::shared_ptr<memory_allocator> The newly created allocator.
     */
    virtual std::shared_ptr<memory_allocator> create_memory_allocator(
        memory_resource &resource
    ) const = 0;

}; 

} // namespace hardware
} // namespace xmipp4
