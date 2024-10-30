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
 * @file device_communicator_manager.hpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Defines device_communicator_manager interface
 * @date 2024-10-29
 * 
 */

#include "../interface_manager.hpp"
#include "../memory/pimpl.hpp"

#include <memory>
#include <vector>

namespace xmipp4 
{
namespace communication
{

class communicator;

} // namespace communication

namespace compute
{

class device;
class device_communicator;
class device_communicator_backend;



/**
 * @brief Centralize multiple device_communicator_backends.
 * 
 */
class device_communicator_manager
    : public interface_manager
{
public:
    using host_communicator = communication::communicator;

    device_communicator_manager();
    device_communicator_manager(const device_communicator_manager &other) = delete;
    device_communicator_manager(device_communicator_manager &&other);
    virtual ~device_communicator_manager();

    device_communicator_manager& operator=(const device_communicator_manager &other) = delete;
    device_communicator_manager& operator=(device_communicator_manager &&other);

    /**
     * @brief Register a new device_communicator backend.
     * 
     * @param backend The backend to be registered.
     * @return true The backend was successfully registered.
     * @return false The backend with the same name already exists.
     * 
     */
    bool register_backend(std::unique_ptr<device_communicator_backend> backend);

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
     * @return device_communicator_backend* The requested backend. 
     * nullptr if not found.
     */
    device_communicator_backend* get_backend(const std::string &name) const;

    /**
     * @brief Find a backend that supports the provided device.
     * 
     * @param dev The device that must be supported.
     * @return device_communicator_backend* A compatible backend.
     * nullptr if none is found.
     */
    device_communicator_backend* find_compatible_backend(const device &dev) const;

    /**
     * @brief Create a communicator for a device.
     * 
     * @param dev The device that will be used in communications.
     * @param comm Host communicator that may be used for supporting
     * device communications. It may get copied.
     * @return std::unique_ptr<device_communicator> The newly created device
     * communicator.
     * @note This method should be called from all ranks of the provided
     * communicator.
     * 
     */
    std::unique_ptr<device_communicator> 
    create_communicator(device &dev,
                        const std::shared_ptr<host_communicator> &comm) const;

    /**
     * @brief Create a communicator for a device.
     * 
     * @param dev The device that will be used in communications.
     * @param comm Host communicator that may be used for supporting
     * device communications. It may get copied.
     * @return std::unique_ptr<device_communicator> The newly created device
     * communicator.
     * @note This method should be called from all ranks of the provided
     * communicator.
     * 
     */
    std::shared_ptr<device_communicator> 
    create_communicator_shared(device &dev,
                               const std::shared_ptr<host_communicator> &comm) const;

private:
    class implementation;
    memory::pimpl<implementation> m_implementation;

}; 

} // namespace compute
} // namespace xmipp4
