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
#include <vector>

namespace xmipp4 
{namespace communication
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

    virtual bool supports_devices(span<device*> devices) const noexcept = 0;

    virtual void create_device_communicators(
        const std::shared_ptr<node_communicator> &node_communicator,
        span<device*> devices,
        std::vector<std::unique_ptr<device_communicator>> &result
    ) const = 0;
    
    virtual void create_device_communicators_shared(
        const std::shared_ptr<node_communicator> &node_communicator,
        span<device*> devices,
        std::vector<std::shared_ptr<device_communicator>> &result 
    ) const = 0;

}; 

} // namespace compute
} // namespace xmipp4
