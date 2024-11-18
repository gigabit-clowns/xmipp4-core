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
 * @file host_transfer.hpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Implementation of device_host_communicator
 * @date 2024-11-06
 * 
 */

#include "../device_to_host_transfer.hpp"
#include "../host_to_device_transfer.hpp"
#include "../device_copy.hpp"

namespace xmipp4 
{
namespace compute
{

/**
 * @brief Implementation of the host_to_device_transfer and 
 * device_to_host_transfer interfaces to allow sending data 
 * to the host acting as a compute device. 
 * 
 * @note Unless the method explicitly indicates a copy, all
 * transfers a zero-copy operation, this is, the input buffer
 * is casted to the output buffer, aliasing its contents.
 * 
 */
class host_transfer final
    : public host_to_device_transfer
    , public device_to_host_transfer
    , public device_copy
{
public:
    void transfer_copy(const std::shared_ptr<const host_buffer> &src_buffer,
                       device_buffer &dst_buffer, 
                       device_queue &queue ) override;
    
    void transfer_copy(const std::shared_ptr<const host_buffer> &src_buffer,
                       device_buffer &dst_buffer, 
                       span<const copy_region> regions, 
                       device_queue &queue ) override;

    std::shared_ptr<device_buffer> 
    transfer(const std::shared_ptr<host_buffer> &buffer, 
             device_memory_allocator &allocator,
             device_queue &queue ) override;

    std::shared_ptr<const device_buffer> 
    transfer(const std::shared_ptr<const host_buffer> &buffer, 
             device_memory_allocator &allocator,
             device_queue &queue ) override;

    void transfer_copy(const device_buffer &src_buffer,
                       const std::shared_ptr<host_buffer> &dst_buffer, 
                       device_queue &queue ) override;

    void transfer_copy(const device_buffer &src_buffer,
                       const std::shared_ptr<host_buffer> &dst_buffer, 
                       span<const copy_region> regions, 
                       device_queue &queue ) override;

    std::shared_ptr<host_buffer> 
    transfer(const std::shared_ptr<device_buffer> &buffer, 
             host_memory_allocator &allocator,
             device_queue &queue ) override;

    std::shared_ptr<const host_buffer> 
    transfer(const std::shared_ptr<const device_buffer> &buffer, 
             host_memory_allocator &allocator,
             device_queue &queue ) override;

    void copy(const device_buffer &src_buffer,
              device_buffer &dst_buffer, 
              device_queue &queue ) override;

    void copy(const device_buffer &src_buffer,
              device_buffer &dst_buffer,
              span<const copy_region> regions,
              device_queue &queue ) override;

    void wait() override;
    void wait(device_queue &queue) override;

}; 

} // namespace compute
} // namespace xmipp4
