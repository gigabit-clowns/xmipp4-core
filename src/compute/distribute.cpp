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
 * @file distribute.cpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Implementation of distribute.hpp
 * @date 2024-10-22
 * 
 */

#include <xmipp4/core/compute/distribute.hpp>

#include <xmipp4/core/compute/communicator.hpp>

#include <cstdlib>

namespace xmipp4
{
namespace compute
{

std::size_t distribute_work(std::size_t work, std::size_t workers, 
                            std::size_t index, std::size_t &start ) noexcept
{
    const auto division = std::div(static_cast<std::ptrdiff_t>(work), workers);
    const std::size_t quotient = division.quot;
    const std::size_t reminder = division.rem;

    std::size_t result;
    if (index < reminder)
    {
        result = quotient + 1;
        start = index*quotient + index;
    }
    else
    {
        result = quotient;
        start = index*quotient + reminder;
    }

    return result;
}

std::size_t distribute_work(std::size_t work, communicator &comm,
                            std::size_t &start )
{
    const auto size = comm.get_size();
    const auto rank = comm.get_rank();
    return distribute_work(work, size, rank, start);    
}

} // namespace system
} // namespace xmipp4
