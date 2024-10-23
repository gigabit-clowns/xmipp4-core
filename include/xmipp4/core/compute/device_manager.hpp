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
 * @date 2024-04-29
 * 
 */

#include "device_descriptor.hpp"

#include <memory>
#include <vector>

namespace xmipp4 
{
namespace compute
{

class device;

class device_manager
{
public:
    device_manager() = default;
    device_manager(const device_manager &other) = default;
    device_manager(device_manager &&other) = default;
    virtual ~device_manager() = default;

    device_manager& operator=(const device_manager &other) = default;
    device_manager& operator=(device_manager &&other) = default;

    virtual std::string get_name() const noexcept = 0;

    virtual void enumerate_devices(std::vector<std::size_t> &ids) const = 0;
    virtual void get_device_descriptor(std::size_t id, device_descriptor &desc) const = 0;

    virtual std::unique_ptr<device> create_device(std::size_t id) = 0;
    virtual std::shared_ptr<device> create_device_shared(std::size_t id) = 0;

}; 

} // namespace compute
} // namespace xmipp4
