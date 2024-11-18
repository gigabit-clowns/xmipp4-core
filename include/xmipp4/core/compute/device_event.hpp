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
 * @file device_event.hpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Defines the compute::device_event interface
 * @date 2024-11-13
 * 
 */

#include "../platform/dynamic_shared_object.h"

namespace xmipp4 
{
namespace compute
{

class device_queue;



/**
 * @brief Abstract class describing a device event.
 * 
 * Instances of this inteface may be used to synchronize the execution
 * of commands across various queues of the same device.
 * 
 * @note When constructing a new event, it is constructed in the signaled
 * state, such that waiting on it does not block execution.
 * 
 */
class XMIPP4_CORE_API device_event
{
public:
    device_event() = default;
    device_event(const device_event &other) = default;
    device_event(device_event &&other) = default;
    virtual ~device_event() = default;

    device_event& operator=(const device_event &other) = default;
    device_event& operator=(device_event &&other) = default;

    /**
     * @brief Signal the event when the current point in the execution 
     * queue is reached.
     * 
     * @param queue The queue where the event is recorded.
     * 
     */
    virtual void signal(device_queue &queue) = 0;

    /**
     * @brief Wait until the last recorded event is signaled by the queue.
     * 
     * @param queue The queue to be blocked until the event is signaled.
     * 
     */
    virtual void wait(device_queue &queue) const = 0;

}; 

} // namespace compute
} // namespace xmipp4
