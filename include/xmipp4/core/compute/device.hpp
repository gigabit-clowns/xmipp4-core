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

class device_queue;
class device_buffer;



/**
 * @brief Abstract representation of a device handle.
 * 
 * This class enables basic interaction functionalities with
 * a compute device.
 * 
 */
class device
{
public:
    device() = default;
    device(const device &other) = default;
    device(device &&other) = default;
    virtual ~device() = default;

    device& operator=(const device &other) = default;
    device& operator=(device &&other) = default;

    /**
     * @brief Create a command queue for this device.
     * 
     * @return std::unique_ptr<device_queue> The queue.
     */
    virtual std::unique_ptr<device_queue> create_queue() = 0;

    /**
     * @brief Create a command queue for this device.
     * 
     * @return std::shared_ptr<device_queue> The queue.
     */
    virtual std::shared_ptr<device_queue> create_queue_shared() = 0;

    /**
     * @brief Allocate a buffer in this device.
     * 
     * @param type Numerical type of the buffer.
     * @param count Number of elements in the buffer.
     * @return std::unique_ptr<device_buffer> The buffer.
     */
    virtual std::unique_ptr<device_buffer> 
    create_buffer(numerical_type type, std::size_t count) = 0;

    /**
     * @brief Allocate a buffer in this device.
     * 
     * @param type Numerical type of the buffer.
     * @param count Number of elements in the buffer.
     * @return std::shared_ptr<device_buffer> The buffer.
     */
    virtual std::shared_ptr<device_buffer> 
    create_buffer_shared(numerical_type type, std::size_t count) = 0;

}; 

} // namespace compute
} // namespace xmipp4
