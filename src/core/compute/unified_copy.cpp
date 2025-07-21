// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/compute/unified_copy.hpp>

#include <xmipp4/core/numerical_type.hpp>
#include <xmipp4/core/compute/unified_buffer.hpp>
#include <xmipp4/core/compute/device_queue.hpp>

#include <stdexcept>
#include <cstring>

namespace xmipp4
{
namespace compute
{

void unified_copy::transfer_copy(const host_buffer &src_buffer, 
                                 device_buffer &dst_buffer, 
                                 device_queue& queue )
{
    copy_unified(
        dynamic_cast<const unified_buffer&>(src_buffer), 
        dynamic_cast<unified_buffer&>(dst_buffer), 
        queue
    );
}

void unified_copy::transfer_copy(const host_buffer &src_buffer, 
                                 device_buffer &dst_buffer, 
                                 span<const copy_region> regions, 
                                 device_queue& queue )
{
    copy_unified(
        dynamic_cast<const unified_buffer&>(src_buffer), 
        dynamic_cast<unified_buffer&>(dst_buffer), 
        regions,
        queue
    );
}

std::shared_ptr<device_buffer> 
unified_copy::transfer(const std::shared_ptr<host_buffer> &buffer, 
                       device_memory_allocator&,
                       std::size_t,
                       device_queue&)
{
    return get_device_accessible_alias(buffer);
}

std::shared_ptr<const device_buffer> 
unified_copy::transfer(const std::shared_ptr<const host_buffer> &buffer, 
                       device_memory_allocator&,
                       std::size_t,
                       device_queue& )
{
    return get_device_accessible_alias(buffer);
}

void unified_copy::transfer_copy(const device_buffer &src_buffer,
                                 host_buffer &dst_buffer,
                                 device_queue& queue )
{
    copy_unified(
        dynamic_cast<const unified_buffer&>(src_buffer), 
        dynamic_cast<unified_buffer&>(dst_buffer), 
        queue
    );
}

void unified_copy::transfer_copy(const device_buffer &src_buffer,
                                 host_buffer &dst_buffer,
                                 span<const copy_region> regions, 
                                 device_queue& queue )
{
    copy_unified(
        dynamic_cast<const unified_buffer&>(src_buffer), 
        dynamic_cast<unified_buffer&>(dst_buffer), 
        regions,
        queue
    );
}

std::shared_ptr<host_buffer> 
unified_copy::transfer(const std::shared_ptr<device_buffer> &buffer, 
                       host_memory_allocator&,
                       std::size_t,
                       device_queue& )
{
    return get_host_accessible_alias(buffer);
}

std::shared_ptr<const host_buffer> 
unified_copy::transfer(const std::shared_ptr<const device_buffer> &buffer, 
                       host_memory_allocator&,
                       std::size_t,
                       device_queue& )
{
    return get_host_accessible_alias(buffer);
}


void unified_copy::copy(const device_buffer &src_buffer,
                        device_buffer &dst_buffer, 
                        device_queue& queue)
{
    copy_unified(
        dynamic_cast<const unified_buffer&>(src_buffer), 
        dynamic_cast<unified_buffer&>(dst_buffer),
        queue
    );
}

void unified_copy::copy(const device_buffer &src_buffer,
                        device_buffer &dst_buffer,
                        span<const copy_region> regions,
                        device_queue& queue)
{
    copy_unified(
        dynamic_cast<const unified_buffer&>(src_buffer), 
        dynamic_cast<unified_buffer&>(dst_buffer), 
        regions,
        queue
    );
}

void unified_copy::copy_unified(const unified_buffer &src_buffer,
                                unified_buffer &dst_buffer, 
                                device_queue& queue )
{
    queue.wait_until_completed();
    compute::copy(src_buffer, dst_buffer);
}

void unified_copy::copy_unified(const unified_buffer &src_buffer,
                        unified_buffer &dst_buffer,
                        span<const copy_region> regions,
                        device_queue& queue )
{
    queue.wait_until_completed();
    compute::copy(src_buffer, dst_buffer, regions);
}
} // namespace compute
} // namespace xmipp4
