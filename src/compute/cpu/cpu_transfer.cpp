// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/numerical_type.hpp>
#include <xmipp4/core/compute/cpu/cpu_transfer.hpp>
#include <xmipp4/core/compute/cpu/cpu_unified_buffer.hpp>

#include <stdexcept>
#include <cstring>

namespace xmipp4
{
namespace compute
{

void cpu_transfer::transfer_copy(const host_buffer &src_buffer, 
                                  device_buffer &dst_buffer, 
                                  device_queue& )
{
    compute::copy(
        src_buffer, 
        dynamic_cast<cpu_unified_buffer&>(dst_buffer)
    );
}

void cpu_transfer::transfer_copy(const host_buffer &src_buffer, 
                                  device_buffer &dst_buffer, 
                                  span<const copy_region> regions, 
                                  device_queue& )
{
    compute::copy(
        src_buffer, 
        dynamic_cast<cpu_unified_buffer&>(dst_buffer), 
        regions
    );
}

std::shared_ptr<device_buffer> 
cpu_transfer::transfer(const std::shared_ptr<host_buffer> &buffer, 
                        device_memory_allocator&,
                        std::size_t,
                        device_queue&)
{
    return get_device_accessible_alias(buffer);
}

std::shared_ptr<const device_buffer> 
cpu_transfer::transfer(const std::shared_ptr<const host_buffer> &buffer, 
                        device_memory_allocator&,
                        std::size_t,
                        device_queue& )
{
    return get_device_accessible_alias(buffer);
}

void cpu_transfer::transfer_copy(const device_buffer &src_buffer,
                                  host_buffer &dst_buffer,
                                  device_queue& )
{
    compute::copy(
        dynamic_cast<const cpu_unified_buffer&>(src_buffer), 
        dst_buffer
    );
}

void cpu_transfer::transfer_copy(const device_buffer &src_buffer,
                                  host_buffer &dst_buffer,
                                  span<const copy_region> regions, 
                                  device_queue& )
{
    compute::copy(
        dynamic_cast<const cpu_unified_buffer&>(src_buffer), 
        dst_buffer,
        regions
    );
}

std::shared_ptr<host_buffer> 
cpu_transfer::transfer(const std::shared_ptr<device_buffer> &buffer, 
                        host_memory_allocator&,
                        std::size_t,
                        device_queue& )
{
    return get_host_accessible_alias(buffer);
}

std::shared_ptr<const host_buffer> 
cpu_transfer::transfer(const std::shared_ptr<const device_buffer> &buffer, 
                        host_memory_allocator&,
                        std::size_t,
                        device_queue& )
{
    return get_host_accessible_alias(buffer);
}


void cpu_transfer::copy(const device_buffer &src_buffer,
                         device_buffer &dst_buffer, 
                         device_queue& )
{
    compute::copy(
        dynamic_cast<const cpu_unified_buffer&>(src_buffer), 
        dynamic_cast<cpu_unified_buffer&>(dst_buffer)
    );
}

void cpu_transfer::copy(const device_buffer &src_buffer,
                         device_buffer &dst_buffer,
                         span<const copy_region> regions,
                         device_queue& )
{
    compute::copy(
        dynamic_cast<const cpu_unified_buffer&>(src_buffer), 
        dynamic_cast<cpu_unified_buffer&>(dst_buffer), 
        regions
    );
}

} // namespace compute
} // namespace xmipp4
