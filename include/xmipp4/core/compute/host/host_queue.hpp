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
 * @file host_queue.hpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Defines host_queue interface
 * @date 2024-10-29
 * 
 */

#include "../queue.hpp"

#include <functional>

namespace xmipp4 
{
namespace compute
{

class device;
class host_device;

/**
 * @brief Special implementation of the queue interface to be able to send
 * commands to the host.
 * 
 */
class host_queue final
    : public queue
{
public:
    host_queue(host_device &device);
    host_queue(const host_queue &other) = default;
    host_queue(host_queue &&other) = default;
    virtual ~host_queue() = default;

    host_queue& operator=(const host_queue &other) = default;
    host_queue& operator=(host_queue &&other) = default;

    device& get_device() const final;

    void synchronize() const final;

private:
    std::reference_wrapper<host_device> m_device;

}; 

} // namespace compute
} // namespace xmipp4
