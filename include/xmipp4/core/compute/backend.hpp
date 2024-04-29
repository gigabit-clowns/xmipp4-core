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
 * @file backend.hpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Defines backend interface
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

class backend
{
public:
    std::vector<device_descriptor> enumerate_devices() const;
    virtual void enumerate_devices(std::vector<device_descriptor>& out) const = 0;
    virtual std::unique_ptr<device> create_device(std::size_t index) const = 0;
    virtual std::shared_ptr<device> create_device_shared(std::size_t index) const = 0;

};

} // namespace compute
} // namespace xmipp4
