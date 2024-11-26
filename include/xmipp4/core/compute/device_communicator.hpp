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
 * @file device_communicator.hpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Defines device_communicator interface
 * @date 2024-11-25
 * 
 */

#include "../platform/dynamic_shared_object.h"

#include <cstddef>

namespace xmipp4 
{
namespace compute
{

class device_memory_allocator;

class XMIPP4_CORE_API device_communicator
{
public:
    device_communicator() = default;
    device_communicator(const device_communicator &other) = default;
    device_communicator(device_communicator &&other) = default;
    virtual ~device_communicator() = default;

    device_communicator& operator=(const device_communicator &other) = default;
    device_communicator& operator=(device_communicator &&other) = default;

    /**
     * @brief Get the amount of peers in the communicator.
     * 
     * @return std::size_t The number of peers.
     * 
     */
    virtual std::size_t get_size() const noexcept = 0;

    /**
     * @brief Get the rank of the local process.
     * 
     * @return int The rank.
     * 
     */
    virtual int get_rank() const noexcept = 0;

    /**
     * @brief Create a device memory allocator optimized for this
     * communicator.
     * 
     * @return std::unique_ptr<device_memory_allocator> The newly created
     * allocator.
     */
    virtual 
    std::unique_ptr<device_memory_allocator>
    create_device_memory_allocator() = 0;

    /**
     * @brief Create a device memory allocator optimized for this
     * communicator.
     * 
     * @return std::shared_ptr<device_memory_allocator> The newly created
     * allocator.
     */
    virtual 
    std::shared_ptr<device_memory_allocator>
    create_device_memory_allocator_shared() = 0;

}; 

} // namespace compute
} // namespace xmipp4
