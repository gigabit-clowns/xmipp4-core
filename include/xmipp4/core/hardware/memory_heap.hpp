// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "../platform/dynamic_shared_object.h"

#include <memory>

namespace xmipp4 
{
namespace hardware
{

class memory_allocation_tracker;
class buffer;

/**
 * @brief Represents a large chunk of memory to serve smaller allocations.
 * 
 */
class XMIPP4_CORE_API memory_heap
{
public:
    memory_heap() = default;
    memory_heap(const memory_heap &other) = default;
    memory_heap(memory_heap &&other) = default;
    virtual ~memory_heap() = default;

    memory_heap& operator=(const memory_heap &other) = default;
    memory_heap& operator=(memory_heap &&other) = default;

    /**
     * @brief Get the size of the allocation.
     * 
     * @return std::size_t Size of the memory heap.
     */
    virtual std::size_t get_size() const noexcept;
    
    /**
     * @brief Allocate a buffer on this memory heap.
     * 
     * The caller must ensure that the requested allocation does not
     * overlap with other previous allocations.
     * 
     * @param offset Position where the buffer is allocated.
     * @param size Size of the allocation.
     * @param tracker Context to track the usage of this buffer. The returned
     * buffer must keep alive this object until destruction.
     * @return std::shared_ptr<buffer> The newly allocated buffer.
     */
    virtual std::shared_ptr<buffer> create_buffer(
        std::size_t offset, 
        std::size_t size,
        std::unique_ptr<memory_allocation_tracker> tracker
    );

}; 

} // namespace hardware
} // namespace xmipp4
