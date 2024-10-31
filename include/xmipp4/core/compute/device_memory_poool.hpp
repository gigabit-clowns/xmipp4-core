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
 * @file device_memory_pool.hpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Defines the compute::device_memory_pool interface
 * @date 2024-10-31
 * 
 */

#include "numerical_type.hpp"
#include "../platform/dynamic_shared_object.h"

#include <memory>
#include <cstddef>

namespace xmipp4 
{
namespace compute
{

class device_buffer;



/**
 * @brief Abstract class defining an in-device memory pool.
 * 
 */
class XMIPP4_CORE_API device_memory_pool
{
public:
    device_memory_pool() = default;
    device_memory_pool(const device_memory_pool &other) = default;
    device_memory_pool(device_memory_pool &&other) = default;
    virtual ~device_memory_pool() = default;

    device_memory_pool& operator=(const device_memory_pool &other) = default;
    device_memory_pool& operator=(device_memory_pool &&other) = default;

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
