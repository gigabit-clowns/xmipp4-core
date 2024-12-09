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
 * @file communicator.cpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Implementation of communicator.hpp
 * @date 2024-12-09
 * 
 */

#include <xmipp4/core/communication/communicator.hpp>

namespace xmipp4
{
namespace communication
{

std::size_t broadcast_string(communicator &comm, int root, std::string &message)
{
    std::size_t count;

    // Broadcast the size
    if (comm.get_rank() == root)
    {
        count = message.size();
    }
    comm.broadcast(root, span<std::size_t>(&count, 1));

    // Broadcast the contents.
    if (count > 0)
    {
        comm.broadcast(
            root, 
            span<std::string::value_type>(
                message.data(), 
                count
            )
        );
    }

    return count;
}

} // namespace communication
} // namespace xmipp4
