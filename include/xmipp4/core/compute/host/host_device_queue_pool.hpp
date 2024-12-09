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
 * @file host_device_queue.hpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Defines host_device_queue interface
 * @date 2024-11-27
 * 
 */

#include "../device_queue_pool.hpp"

#include "host_device_queue.hpp"

namespace xmipp4 
{
namespace compute
{

/**
 * @brief Implementation of the device_queue_pool interface to be 
 * able to obtain host_device_queue-s.
 * 
 * As host_device_queue-s are synchronous, it does not make sense to
 * instantiate multiple of them. Thus, this pool always has a size of
 * one.
 * 
 */
class host_device_queue_pool final
    : public device_queue_pool
{
public:
    std::size_t get_size() const noexcept override;
    host_device_queue& get_queue(std::size_t index) override;

private:
    host_device_queue m_queue;

}; 

} // namespace compute
} // namespace xmipp4
