// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <cstddef>
#include <typeinfo>

namespace xmipp4 
{
namespace compute
{

class memory_resource;

class buffer
{
public:
    buffer() = default;
    buffer(const buffer &other) = default;
    buffer(buffer &&other) = default;
    virtual ~buffer() = default;

    buffer& operator=(const buffer &other) = default;
    buffer& operator=(buffer &&other) = default;

    /**
     * @brief Get a host accessible pointer to the data.
     * 
     * This method only returns a pointer if the data is accessible by the 
     * host, i.e., if the kind of the underlying memory_resource is one of:
     * device_mapped, host_staging, unified or managed.
     * 
     * @return void* Pointer to the data. nullptr if the buffer is not
     * host accessible.
     */
    virtual void* get_host_ptr() noexcept;

    /**
     * @brief Get a host accessible pointer to the data.
     * 
     * This method only returns a pointer if the data is accessible by the 
     * host, i.e., if the kind of the underlying memory_resource is one of:
     * device_mapped, host_staging, unified or managed.
     * 
     * @return void* Pointer to the data. nullptr if the buffer is not
     * host accessible.
     */
    virtual const void* get_host_ptr() const noexcept;

    /**
     * @brief Get the size in bytes for this buffer.
     * 
     * @return std::size_t Size in bytes.
     */
    virtual std::size_t get_size() const noexcept = 0;

    /**
     * @brief Get the memory_resource where this buffer is stored. 
     * 
     * @return memory_resource& The resource where the buffer is stored.
     */
    virtual memory_resource& get_memory_resource() const noexcept = 0;

};

} // namespace compute
} // namespace xmipp4
