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
 * @file device_queue.hpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Defines the compute::device_queue interface
 * @date 2024-10-22
 * 
 */

#include "../platform/dynamic_shared_object.h"

namespace xmipp4 
{
namespace compute
{

/**
 * @brief Abstract class describing a command device_queue.
 * 
 */
class XMIPP4_CORE_API device_queue
{
public:
    device_queue() = default;
    device_queue(const device_queue &other) = default;
    device_queue(device_queue &&other) = default;
    virtual ~device_queue() = default;

    device_queue& operator=(const device_queue &other) = default;
    device_queue& operator=(device_queue &&other) = default;

    /**
     * @brief Wait until the device_queue is flushed.
     * 
     */
    virtual void wait_until_completed() const = 0;

}; 

} // namespace compute
} // namespace xmipp4
