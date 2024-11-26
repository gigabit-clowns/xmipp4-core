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
 * @file device_communicator_backend.hpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Defines device_communicator_backend interface
 * @date 2024-11-25
 * 
 */

#include "../backend.hpp"
#include "../span.hpp"
#include "../platform/dynamic_shared_object.h"

#include <memory>

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

/**
 * @brief Abstract class that represents a device communications backend.
 * 
 * This interface provides allows obtaining a device_communicator object
 * for a device group.
 * 
 */
class XMIPP4_CORE_API device_communicator_backend
    : public backend
{
public:
    using node_communicator = communication::communicator;

    /**
     * @brief Check if this backend supports creating communicators 
     * for the provided devices.
     * 
     * @param devices The devices to be checked.
     * @return true This backend can create communicators for 
     * all provided devices.
     * @return false This backend cannot create communicators for one
     * or more devices.
     */
    virtual bool supports_devices(span<device*> devices) const noexcept = 0;

    /**
     * @brief Create a device communicators object
     * 
     * @param node_communicator Node communicator used during the creation.
     * @param devices Devices for which the communicators are created.
     * @param result Output list with the newly created communicators. Must 
     * have the same size as the device array.
     */
    virtual 
    void create_device_communicators(node_communicator &node_communicator,
                                     span<device*> devices,
                                     span<std::unique_ptr<device_communicator>> &result ) const = 0;
    
    /**
     * @brief Create a device communicators object
     * 
     * @param node_communicator Node communicator used during the creation.
     * @param devices Devices for which the communicators are created.
     * @param result Output list with the newly created communicators. Must 
     * have the same size as the device array.
     */
    virtual 
    void create_device_communicators_shared(node_communicator &node_communicator,
                                            span<device*> devices,
                                            span<std::shared_ptr<device_communicator>> &result ) const = 0;

}; 

} // namespace compute
} // namespace xmipp4
