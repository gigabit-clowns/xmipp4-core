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
 * @file device_manager.hpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Defines device_manager interface
 * @date 2024-10-23
 * 
 */

#include "device_index.hpp"
#include "device_properties.hpp"
#include "device_backend.hpp"
#include "../backend_manager.hpp"
#include "../platform/dynamic_shared_object.h"

#include <memory>
#include <vector>

namespace xmipp4 
{
namespace compute
{

class device;
class device_backend;
class device_create_parameters;



/**
 * @brief Centralize multiple device_backends.
 * 
 */
class device_manager final
    : public basic_backend_manager<device_backend>
{
public:
    device_manager() = default;
    device_manager(const device_manager &other) = delete;
    device_manager(device_manager &&other) = default;
    ~device_manager() override = default;

    device_manager& operator=(const device_manager &other) = delete;
    device_manager& operator=(device_manager &&other) = default;

    XMIPP4_CORE_API
    void register_builtin_backends() override;

    /**
     * @brief Enumerate devices across all backends.
     * 
     * @param indices Output list of device indices.
     * @note The output list is cleared before populating it.
     * 
     */
    XMIPP4_CORE_API
    void enumerate_devices(std::vector<device_index> &indices) const;

    /**
     * @brief Query the properties of a device.
     * 
     * @param index The index of the device.
     * @param desc Output device properties.
     * @return true Device exists and properties were written.
     * @return false Device does not exist and properties were not written.
     */
    XMIPP4_CORE_API
    bool get_device_properties(const device_index &index, 
                               device_properties &desc ) const;

    /**
     * @brief Create a device handle.
     * 
     * @param index Index of the device.
     * @param params Parameters used for device instantiation.
     * @return std::shared_ptr<device> The device handle.
     */
    XMIPP4_CORE_API std::shared_ptr<device> 
    create_device(const device_index &index,
                  const device_create_parameters &params ) const;

}; 

} // namespace compute
} // namespace xmipp4
