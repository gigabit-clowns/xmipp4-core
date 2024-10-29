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
 * @file device.hpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Defines the compute::device interface
 * @date 2024-10-22
 * 
 */

#include <memory>

#include "numerical_type.hpp"

namespace xmipp4 
{
namespace compute
{

class queue;
class device_buffer;

class device
{
public:
    device() = default;
    device(const device &other) = default;
    device(device &&other) = default;
    virtual ~device() = default;

    device& operator=(const device &other) = default;
    device& operator=(device &&other) = default;

    virtual std::unique_ptr<queue> create_queue() = 0;
    virtual std::shared_ptr<queue> create_queue_shared() = 0;

    virtual std::unique_ptr<device_buffer> 
    create_buffer(numerical_type type, std::size_t count) = 0;
    virtual std::shared_ptr<device_buffer> 
    create_buffer_shared(numerical_type type, std::size_t count) = 0;

}; 

} // namespace compute
} // namespace xmipp4
