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
 * @file host_device_communicator.cpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Implementation of host_device_communicator.hpp
 * @date 2024-10-29
 * 
 */

#include <xmipp4/core/compute/host/host_device_communicator.hpp>

#include <xmipp4/core/communication/communicator.hpp>

namespace xmipp4
{
namespace compute
{

host_device_communicator::host_device_communicator(std::shared_ptr<host_communicator> comm)
    : m_communicator(std::move(comm))
{
}

std::size_t host_device_communicator::get_size() const
{
    return m_communicator->get_size();
}

int host_device_communicator::get_rank() const
{
    return m_communicator->get_rank();
}

std::unique_ptr<device_buffer>
host_device_communicator::create_buffer(numerical_type type,
                                        std::size_t count )
{
    // TODO
}

std::shared_ptr<device_buffer> 
host_device_communicator::create_buffer_shared(numerical_type type,
                                               std::size_t count)
{
    // TODO
}

void host_device_communicator::barrier(queue &q)
{
    m_communicator->barrier();
}

void host_device_communicator::send(int destination_rank, 
                                    const device_buffer &buf, queue &q )
{

}

void host_device_communicator::receive(int source_rank, 
                                       device_buffer &buf, queue &q )
{
    // TODO
}

void host_device_communicator::broadcast(int root, device_buffer &buf, queue &q)
{
    // TODO
}

void host_device_communicator::scatter(int root, 
                                       const device_buffer &send_buf, 
                                       device_buffer &recv_buf, 
                                       queue &q )
{
    // TODO
}

void host_device_communicator::gather(int root, 
                                      const device_buffer &send_buf, 
                                      device_buffer &recv_buf, 
                                      queue &q )
{
    // TODO
}

void host_device_communicator::all_gather(const device_buffer &send_buf, 
                                          device_buffer &recv_buf, 
                                          queue &q )
{
    // TODO
}

void host_device_communicator::reduce(int root, reduction_operation op,
                                      const device_buffer &send_buf, 
                                      device_buffer &recv_buf )
{
    // TODO
}

void host_device_communicator::all_reduce(reduction_operation op,
                                          const device_buffer &send_buf, 
                                          device_buffer &recv_buf )
{
    // TODO
}

void host_device_communicator::all_to_all(const device_buffer &send_buf, 
                                          device_buffer &recv_buf, 
                                          queue &q )
{
    // TODO
}

} // namespace system
} // namespace xmipp4
