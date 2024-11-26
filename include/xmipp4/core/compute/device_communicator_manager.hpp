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
 * @date 2024-11-27
 * 
 */

#include "../interface_manager.hpp"
#include "../span.hpp"
#include "../memory/pimpl.hpp"
#include "../platform/dynamic_shared_object.h"

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
class device_communicator_manager final
    : public interface_manager
{
public:
    using node_communicator = communication::communicator;

    XMIPP4_CORE_API device_communicator_manager();
    device_communicator_manager(const device_communicator_manager &other) = delete;
    XMIPP4_CORE_API device_communicator_manager(device_communicator_manager &&other) noexcept;
    XMIPP4_CORE_API virtual ~device_communicator_manager() override;

    device_communicator_manager& 
    operator=(const device_communicator_manager &other) = delete;
    XMIPP4_CORE_API device_communicator_manager& 
    operator=(device_communicator_manager &&other) noexcept;

    /**
     * @brief Register a new device communicator backend.
     * 
     * @param backend The backend to be registered.
     * @return true The backend was successfully registered.
     * @return false The backend with the same name already exists.
     * 
     */
    XMIPP4_CORE_API
    bool register_backend(std::unique_ptr<device_communicator_backend> backend);

    /**
     * @brief Get a backend by its name.
     * 
     * @param name The name of the backend.
     * @return device_communicator_backend* The requested backend. 
     * nullptr if not found.
     */
    XMIPP4_CORE_API
    device_communicator_backend* get_backend(const std::string &name) const;

    /**
     * @brief Find the most suitable backend that supports a set of devices.
     * 
     * The most suitable backend is an available backend with the highest
     * priority that supports the provided device set.
     * 
     * @param devices 
     * @return device_communicator_backend* The requested backend. 
     * nullptr if not found.
     */
    XMIPP4_CORE_API
    device_communicator_backend* 
    find_supported_backend(span<device*> devices) const;

    /**
     * @brief Create a device communicators from the most suitable backend.
     * 
     * @param node_communicator Node communicator used during the creation.
     * @param devices Devices for which the communicators are created. 
     * Must be supported.
     * @param result Output list with the newly created communicators. Must 
     * have the same size as the device array.
     * @return true Communicators were successfully created.
     * @return false No suitable backend was found.
     */
    XMIPP4_CORE_API
    bool create_device_communicators(node_communicator &node_communicator,
                                     span<device*> devices,
                                     span<std::unique_ptr<device_communicator>> &result ) const;
    
    /**
     * @brief Create a device communicators from the most suitable backend.
     * 
     * @param node_communicator Node communicator used during the creation.
     * @param devices Devices for which the communicators are created.
     * Must be supported.
     * @param result Output list with the newly created communicators. Must 
     * have the same size as the device array.
     * @return true Communicators were successfully created.
     * @return false No suitable backend was found.
     */
    XMIPP4_CORE_API
    bool create_device_communicators_shared(node_communicator &node_communicator,
                                            span<device*> devices,
                                            span<std::shared_ptr<device_communicator>> &result ) const;

private:
    class implementation;
    memory::pimpl<implementation> m_implementation;

}; 

} // namespace compute
} // namespace xmipp4
