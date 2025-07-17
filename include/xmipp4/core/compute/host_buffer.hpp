// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "copy_region.hpp"
#include "../span.hpp"
#include "../platform/dynamic_shared_object.h"

#include <memory>
#include <cstddef>

namespace xmipp4 
{
namespace compute
{

class device_buffer;
class device_queue;

/**
 * @brief Abstract class defining an in-host memory
 * allocation.
 * 
 */
class XMIPP4_CORE_API host_buffer
{
public:
    host_buffer() = default;
    host_buffer(const host_buffer &other) = default;
    host_buffer(host_buffer &&other) = default;
    virtual ~host_buffer() = default;

    host_buffer& operator=(const host_buffer &other) = default;
    host_buffer& operator=(host_buffer &&other) = default;

    /**
     * @brief Get the number of bytes in the buffer.
     * 
     * @return std::size_t The size of the buffer.
     * 
     */
    virtual std::size_t get_size() const noexcept = 0;

    /**
     * @brief Get the data.
     * 
     * @return void* The data.
     */
    virtual void* get_data() noexcept = 0;

    /**
     * @brief Get the data.
     * 
     * @return const void* The data.
     */
    virtual const void* get_data() const noexcept = 0;

    /**
     * @brief Get a device accessible alias of this buffer.
     * 
     * If this buffer is not device accessible, this method returns null.
     * 
     * @return device_buffer* Device accessible alias of this buffer.
     * 
     */
    virtual device_buffer* get_device_accessible_alias() noexcept = 0;

    /**
     * @brief Get a device accessible alias of this buffer.
     * 
     * If this buffer is not device accessible, this method returns null.
     * 
     * @return const device_buffer* Device accessible alias of this buffer.
     */
    virtual 
    const device_buffer* get_device_accessible_alias() const noexcept = 0;

    /**
     * @brief Acknowledge that the buffer is being used in a queue other than
     * the one used for allocation.
     * 
     * @param queue The queue where the buffer is being used.
     * 
     */
    virtual void record_queue(device_queue &queue) = 0;

};



/**
 * @brief Get a device accessible alias of a host_buffer.
 * 
 * If this buffer is not device accessible, this method returns null.
 * 
 * @param buffer Buffer to be aliased. May be null, in which case null is
 * returned.
 * @return std::shared<device_buffer> Device accessible alias of the
 * provided buffer.
 */
XMIPP4_CORE_API
std::shared_ptr<device_buffer> 
get_device_accessible_alias(const std::shared_ptr<host_buffer> &buffer) noexcept;

/**
 * @brief Get a device accessible alias of a host_buffer.
 * 
 * If this buffer is not device accessible, this method returns null.
 * 
 * @param buffer Buffer to be aliased. May be null, in which case null is
 * returned.
 * @return std::shared<const device_buffer> Device accessible alias of the
 * provided buffer.
 */
XMIPP4_CORE_API
std::shared_ptr<const device_buffer> 
get_device_accessible_alias(const std::shared_ptr<const host_buffer> &buffer) noexcept;

/**
 * @brief Copy the contents of one buffer to another.
 * 
 * @param src_buffer Buffer from which elements are copied.
 * @param dst_buffer Buffer to which elements are copied.
 * 
 * @note Both buffers must have the same size.
 * 
 */
XMIPP4_CORE_API
void copy(const host_buffer &src_buffer, host_buffer &dst_buffer);

/**
 * @brief Copy regions of one buffer to another.
 * 
 * @param src_buffer Buffer from which elements are copied.
 * @param dst_buffer Buffer to which elements are copied.
 * @param regions Regions to be copied.
 * 
 * @note All values in regions are expressed in terms of bytes.
 * @note For all regions, source_offset+count must not be greater
 * than the source buffer size and destination_offset+count
 * must not be greater than the destination buffer size.
 * @note Regions should not overlap within a domain.
 * 
 */
XMIPP4_CORE_API
void copy(const host_buffer &src_buffer, host_buffer &dst_buffer,
          span<const copy_region> regions );

} // namespace compute
} // namespace xmipp4
