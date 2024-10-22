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

namespace xmipp4
{
namespace compute
{

std::size_t distribute_work(communicator &comm, std::size_t count, 
                            std::size_t &start, std::size_t &end )
{
    const auto size = comm.get_size();
    const auto rank = comm.get_rank();
    
    const auto quot = count / size;
    const auto rem = count % size;

    std::size_t result;
    if (rank < rem)
    {
        result = quot + 1;
        start = rank*quot + rank;
    }
    else
    {
        result = quot;
        start = rank*quot + rem;
    }

    end = start + result;
    return result;
}

} // namespace system
} // namespace xmipp4
