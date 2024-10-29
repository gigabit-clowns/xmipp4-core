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
 * @date 2024-10-29
 * 
 */


#include "../backend.hpp"

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
 * @brief Abstract class that represents a device communicator.
 * 
 */
class device_communicator_backend
    : public backend
{
public:
    using host_communicator = communication::communicator;

    device_communicator_backend() = default;
    device_communicator_backend(const device_communicator_backend &other) = default;
    device_communicator_backend(device_communicator_backend &&other) = default;
    virtual ~device_communicator_backend() = default;

    device_communicator_backend& operator=(const device_communicator_backend &other) = default;
    device_communicator_backend& operator=(device_communicator_backend &&other) = default;

    virtual bool supports_device(const device &dev) const noexcept = 0;

    virtual std::unique_ptr<device_communicator> 
    create_communicator(device &dev,
                        const std::shared_ptr<host_communicator> &comm) const = 0;
    virtual std::shared_ptr<device_communicator> 
    create_communicator_shared(device &dev,
                               const std::shared_ptr<host_communicator> &comm) const = 0;

}; 

} // namespace compute
} // namespace xmipp4
