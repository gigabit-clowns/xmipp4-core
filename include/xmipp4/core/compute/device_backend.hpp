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

#include <memory>
#include <vector>

namespace xmipp4 
{
namespace compute
{

class device;

class device_backend
    : public backend
{
public:
    device_backend() = default;
    device_backend(const device_backend &other) = default;
    device_backend(device_backend &&other) = default;
    virtual ~device_backend() = default;

    device_backend& operator=(const device_backend &other) = default;
    device_backend& operator=(device_backend &&other) = default;

    virtual void enumerate_devices(std::vector<std::size_t> &ids) const = 0;
    virtual bool get_device_properties(std::size_t id, device_properties &desc) const = 0;

    virtual std::unique_ptr<device> create_device(std::size_t id) = 0;
    virtual std::shared_ptr<device> create_device_shared(std::size_t id) = 0;

}; 

} // namespace compute
} // namespace xmipp4
