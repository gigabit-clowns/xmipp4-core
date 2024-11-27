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
 * @file device_queue_pool.hpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Defines the compute::device_queue_pool interface
 * @date 2024-11-27
 * 
 */

#include "../platform/dynamic_shared_object.h"

#include <cstddef>

namespace xmipp4 
{
namespace compute
{

class device_queue;

/**
 * @brief Abstract class describing a pool of device_queues.
 * 
 */
class XMIPP4_CORE_API device_queue_pool
{
public:
    device_queue_pool() = default;
    device_queue_pool(const device_queue_pool &other) = default;
    device_queue_pool(device_queue_pool &&other) = default;
    virtual ~device_queue_pool() = default;

    device_queue_pool& operator=(const device_queue_pool &other) = default;
    device_queue_pool& operator=(device_queue_pool &&other) = default;

    virtual std::size_t get_size() const noexcept = 0;
    virtual device_queue& get_queue(std::size_t index) = 0;

}; 

} // namespace compute
} // namespace xmipp4

