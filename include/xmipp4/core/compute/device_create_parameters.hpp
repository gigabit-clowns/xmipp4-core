// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "../platform/constexpr.hpp"

#include <cstddef>

namespace xmipp4
{
namespace compute
{

/**
 * @brief Description of resources requested to create a device handle.
 * 
 */
class device_create_parameters
{
public:
    XMIPP4_CONSTEXPR device_create_parameters() noexcept;
    device_create_parameters(const device_create_parameters& other) = default;
    device_create_parameters(device_create_parameters&& other) = default;
    ~device_create_parameters() = default;

    device_create_parameters&
    operator=(const device_create_parameters& other) = default;
    device_create_parameters&
    operator=(device_create_parameters&& other) = default;

    /**
     * @brief Set the desired queue count.
     * 
     * @param count Requested number of queues.
     * 
     * @note This number is a request. The actual number of queues created
     * in the device will depend on the device/backend capabilities. Do not
     * assume that this will be the numer of queues available for submiting
     * jobs. Instead, use the number returned by device_queue_pool::get_size().
     * 
     */
    XMIPP4_CONSTEXPR void set_desired_queue_count(std::size_t count) noexcept;

    /**
     * @brief Get the desired queue count.
     * 
     * By default, this number will be 1.
     * 
     * @return std::size_t Requested number of queues.
     */
    XMIPP4_CONSTEXPR std::size_t get_desired_queue_count() const noexcept;

private:
    std::size_t m_desired_queue_count;

};

} // namespace compute
} // namespace xmipp4

#include "device_create_parameters.inl"
