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
 * @file device_communicator.hpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Definition of the compute::device_communicator class
 * @date 2024-10-21
 * 
 */

#include "reduction_operation.hpp"
#include "numerical_type.hpp"

#include <memory>
#include <cstddef>

namespace xmipp4 
{
namespace compute
{

class device;
class device_buffer;
class queue;

class device_communicator
{
public:
    device_communicator() = default;
    device_communicator(const device_communicator &other) = default;
    device_communicator(device_communicator &&other) = default;
    virtual ~device_communicator() = default;

    device_communicator& operator=(const device_communicator &other) = default;
    device_communicator& operator=(device_communicator &&other) = default;

    virtual std::size_t get_size() const = 0;
    virtual int get_rank() const = 0;

    virtual device& get_device() const noexcept = 0;
    
    virtual std::unique_ptr<device_buffer>
    create_buffer(numerical_type type,
                  std::size_t count) = 0;
    virtual std::shared_ptr<device_buffer> 
    create_buffer_shared(numerical_type type,
                         std::size_t count) = 0;

    virtual void barrier() = 0;

    virtual void send(int destination_rank, 
                      const device_buffer &buf, queue &q) = 0;

    virtual void receive(int source_rank, device_buffer &buf, queue &q) = 0;

    virtual void broadcast(int root, device_buffer &buf, queue &q) = 0;

    virtual void scatter(int root, 
                         const device_buffer &send_buf, device_buffer &recv_buf, 
                         queue &q ) = 0;

    virtual void gather(int root, 
                        const device_buffer &send_buf, device_buffer &recv_buf, 
                        queue &q ) = 0;

    virtual void all_gather(const device_buffer &send_buf, device_buffer &recv_buf, 
                            queue &q) = 0;

    virtual void reduce(int root, reduction_operation op,
                        const device_buffer &send_buf, device_buffer &recv_buf ) = 0;

    virtual void all_reduce(reduction_operation op,
                            const device_buffer &send_buf, device_buffer &recv_buf ) = 0;

    virtual void all_to_all(const device_buffer &send_buf, device_buffer &recv_buf, 
                            queue &q) = 0;

};

} // namespace compute
} // namespace xmipp4
