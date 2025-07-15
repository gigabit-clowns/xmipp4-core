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
 * @file cpu_device_queue_pool.cpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Implementation of cpu_device_queue_pool.hpp
 * @date 2024-11-27
 * 
 */

#include <xmipp4/core/compute/cpu/cpu_device_queue_pool.hpp>

#include <stdexcept>

namespace xmipp4
{
namespace compute
{


std::size_t cpu_device_queue_pool::get_size() const noexcept
{
    return 1;
}

cpu_device_queue& cpu_device_queue_pool::get_queue(std::size_t index)
{
    if (index > 0)
    {
        throw std::out_of_range(
            "queue index is out of range"
        );
    }

    return m_queue;
}

} // namespace compute
} // namespace xmipp4
