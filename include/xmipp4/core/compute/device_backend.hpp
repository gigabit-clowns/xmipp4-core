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

/**
 * @file device_backend.hpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Defines device_backend interface
 * @date 2024-10-23
 * 
 */

#include "device_properties.hpp"
#include "../backend.hpp"
#include "../platform/dynamic_shared_object.h"

#include <memory>
#include <vector>

namespace xmipp4 
{
namespace compute
{

class device;

/**
 * @brief Abstract class that represents a compute backend.
 * 
 * This interface provides basic methods for querying and
 * obtaining handles to compute devices.
 * 
 */
class XMIPP4_CORE_API device_backend
    : public backend
{
public:
    device_backend() = default;
    device_backend(const device_backend &other) = default;
    device_backend(device_backend &&other) = default;
    virtual ~device_backend() = default;

    device_backend& operator=(const device_backend &other) = default;
    device_backend& operator=(device_backend &&other) = default;

    /**
     * @brief Enumerate available in devices within this backend.
     * 
     * @param ids IDs of the devices. 
     * @note The IDs are only unique within each backend.
     * 
     */
    virtual void enumerate_devices(std::vector<std::size_t> &ids) const = 0;

    /**
     * @brief Query the properties of a device.
     * 
     * @param id Identifier of the device.
     * @param desc Output properties.
     * @return true The device ID exists. Properties were written.
     * @return false The device ID does not exist. Properties were not written.
     * 
     */
    virtual bool get_device_properties(std::size_t id, device_properties &desc) const = 0;

    /**
     * @brief Create a device handle for the given device identifier.
     * 
     * @param id The identifier of the device.
     * @return std::unique_ptr<device> The device handle.
     */
    virtual std::unique_ptr<device> create_device(std::size_t id) = 0;

    /**
     * @brief Create a device handle for the given device identifier.
     * 
     * @param id The identifier of the device.
     * @return std::shared_ptr<device> The device handle.
     */
    virtual std::shared_ptr<device> create_device_shared(std::size_t id) = 0;

}; 

} // namespace compute
} // namespace xmipp4
