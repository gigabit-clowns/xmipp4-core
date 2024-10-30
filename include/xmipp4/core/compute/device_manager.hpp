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
#include "../interface_manager.hpp"
#include "../memory/pimpl.hpp"

#include <memory>
#include <vector>

namespace xmipp4 
{
namespace compute
{

class device;
class device_backend;



/**
 * @brief Centralize multiple device_backends.
 * 
 */
class device_manager
    : public interface_manager
{
public:
    device_manager();
    device_manager(const device_manager &other) = delete;
    device_manager(device_manager &&other);
    virtual ~device_manager();

    device_manager& operator=(const device_manager &other) = delete;
    device_manager& operator=(device_manager &&other);

    /**
     * @brief Register a new device backend.
     * 
     * @param backend The backend to be registered.
     * @return true The backend was successfully registered.
     * @return false The backend with the same name already exists.
     * 
     */
    bool register_backend(std::unique_ptr<device_backend> backend);

    /**
     * @brief Enumerate all registered backends.
     * 
     * @param backends Output list with the backend names.
     * @note The output list is cleared before populating it.
     * 
     */
    void enumerate_backends(std::vector<std::string> &backends) const;

    /**
     * @brief Get a backend by its name.
     * 
     * @param name The name of the backend.
     * @return device_backend* The requested backend. nullptr if not found.
     */
    device_backend* get_backend(const std::string &name) const;

    /**
     * @brief Enumerate devices across all backends.
     * 
     * @param indices Output list of device indices.
     * @note The output list is cleared before populating it.
     * 
     */
    void enumerate_devices(std::vector<device_index> &indices) const;

    /**
     * @brief Query the properties of a device.
     * 
     * @param index The index of the device.
     * @param desc Output device properties.
     * @return true Device exists and properties were written.
     * @return false Device does not exist and properties were not written.
     */
    bool get_device_properties(const device_index &index, 
                               device_properties &desc ) const;

    /**
     * @brief Create a device handle
     * 
     * @param index Index of the device.
     * @return std::unique_ptr<device> The device handle.
     */
    std::unique_ptr<device> 
    create_device(const device_index &index) const;

    /**
     * @brief Create a device handle
     * 
     * @param index Index of the device.
     * @return std::shared_ptr<device> The device handle.
     */
    std::shared_ptr<device> 
    create_device_shared(const device_index &index) const;

private:
    class implementation;
    memory::pimpl<implementation> m_implementation;

}; 

} // namespace compute
} // namespace xmipp4
