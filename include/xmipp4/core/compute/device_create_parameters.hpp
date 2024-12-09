#pragma once

/***************************************************************************
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
 * 02111-1307  USA
 *
 *  All comments concerning this program package may be sent to the
 *  e-mail address 'xmipp@cnb.csic.es'
 ***************************************************************************/

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
