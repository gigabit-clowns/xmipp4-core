// SPDX-License-Identifier: GPL-3.0-only

#pragma once

/**
 * @file cpu_transfer.hpp
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
class cpu_transfer final
    : public host_to_device_transfer
    , public device_to_host_transfer
    , public device_copy
{
public:
    void transfer_copy(const host_buffer &src_buffer,
                       device_buffer &dst_buffer, 
                       device_queue &queue ) override;
    
    void transfer_copy(const host_buffer &src_buffer,
                       device_buffer &dst_buffer, 
                       span<const copy_region> regions, 
                       device_queue &queue ) override;

    std::shared_ptr<device_buffer> 
    transfer(const std::shared_ptr<host_buffer> &buffer, 
             device_memory_allocator &allocator,
             std::size_t alignment,
             device_queue &queue ) override;

    std::shared_ptr<const device_buffer> 
    transfer(const std::shared_ptr<const host_buffer> &buffer, 
             device_memory_allocator &allocator,
             std::size_t alignment,
             device_queue &queue ) override;

    void transfer_copy(const device_buffer &src_buffer,
                       host_buffer &dst_buffer, 
                       device_queue &queue ) override;

    void transfer_copy(const device_buffer &src_buffer,
                       host_buffer &dst_buffer, 
                       span<const copy_region> regions, 
                       device_queue &queue ) override;

    std::shared_ptr<host_buffer> 
    transfer(const std::shared_ptr<device_buffer> &buffer, 
             host_memory_allocator &allocator,
             std::size_t alignment,
             device_queue &queue ) override;

    std::shared_ptr<const host_buffer> 
    transfer(const std::shared_ptr<const device_buffer> &buffer, 
             host_memory_allocator &allocator,
             std::size_t alignment,
             device_queue &queue ) override;

    void copy(const device_buffer &src_buffer,
              device_buffer &dst_buffer, 
              device_queue &queue ) override;

    void copy(const device_buffer &src_buffer,
              device_buffer &dst_buffer,
              span<const copy_region> regions,
              device_queue &queue ) override;

}; 

} // namespace compute
} // namespace xmipp4
