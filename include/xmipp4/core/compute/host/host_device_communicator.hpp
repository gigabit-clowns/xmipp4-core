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
 * @file host_device_communicator.hpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Defines host_device_communicator interface
 * @date 2024-10-29
 * 
 */

#include "../device_communicator.hpp"

#include <memory>

namespace xmipp4 
{
namespace compute
{
namespace communication
{

class communicator;

} // namespace communication



/**
 * @brief Special implementation of the device_communicator interface to be able 
 * to wrap the communication::communicator as a device_communicator.
 * 
 */
class host_device_communicator final
    : public device_communicator
{
public:
    using host_communicator = xmipp4::communication::communicator;

    host_device_communicator() = default;
    host_device_communicator(std::shared_ptr<host_communicator> comm);
    host_device_communicator(const host_device_communicator &other) = default;
    host_device_communicator(host_device_communicator &&other) = default;
    virtual ~host_device_communicator() = default;

    host_device_communicator& operator=(const host_device_communicator &other) = default;
    host_device_communicator& operator=(host_device_communicator &&other) = default;

    std::size_t get_size() const final;
    int get_rank() const final;

    std::unique_ptr<device_buffer>
    create_buffer(numerical_type type,
                  std::size_t count) final;
    std::shared_ptr<device_buffer> 
    create_buffer_shared(numerical_type type,
                         std::size_t count) final;

    void barrier(queue &q) final;

    void send(int destination_rank, 
              const device_buffer &buf, queue &q) final;

    void receive(int source_rank, device_buffer &buf, queue &q) final;
    
    void send_receive(int destination_rank, 
                      const device_buffer &send_buf,
                      int source_rank,
                      device_buffer &recv_buf, 
                      queue &q ) final;

    void broadcast(int root, device_buffer &buf, queue &q) final;

    void scatter(int root, 
                 const device_buffer &send_buf, 
                 device_buffer &recv_buf, 
                 queue &q ) final;

    void gather(int root, 
                const device_buffer &send_buf, 
                device_buffer &recv_buf, 
                queue &q ) final;

    void all_gather(const device_buffer &send_buf, 
                    device_buffer &recv_buf, 
                    queue &q ) final;

    void reduce(int root, reduction_operation operation,
                const device_buffer &send_buf, 
                device_buffer &recv_buf,
                queue &q ) final;

    void all_reduce(reduction_operation operation,
                    const device_buffer &send_buf, 
                    device_buffer &recv_buf,
                    queue &q ) final;

    void all_to_all(const device_buffer &send_buf, 
                    device_buffer &recv_buf, 
                    queue &q ) final;

private:
    std::shared_ptr<host_communicator> m_communicator;

}; 

} // namespace compute
} // namespace xmipp4
