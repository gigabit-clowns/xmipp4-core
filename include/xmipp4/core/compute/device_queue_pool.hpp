// SPDX-License-Identifier: GPL-3.0-only

#pragma once

/**
 * @file device_queue_pool.hpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Defines the compute::device_queue_pool interface
 * @date 2024-11-27
 * 
 */

#include "../platform/dynamic_shared_object.h"

#include <cstddef>

namespace xmipp4 
{
namespace compute
{

class device_queue;

/**
 * @brief Abstract class describing a pool of device_queues.
 * 
 */
class XMIPP4_CORE_API device_queue_pool
{
public:
    device_queue_pool() = default;
    device_queue_pool(const device_queue_pool &other) = default;
    device_queue_pool(device_queue_pool &&other) = default;
    virtual ~device_queue_pool() = default;

    device_queue_pool& operator=(const device_queue_pool &other) = default;
    device_queue_pool& operator=(device_queue_pool &&other) = default;

    virtual std::size_t get_size() const noexcept = 0;
    virtual device_queue& get_queue(std::size_t index) = 0;

}; 

} // namespace compute
} // namespace xmipp4

