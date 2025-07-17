// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/compute/host_buffer.hpp>

#include <xmipp4/core/compute/checks.hpp>
#include <xmipp4/core/memory/align.hpp>

#include <stdexcept>
#include <cstring>

namespace xmipp4
{
namespace compute
{

template <typename DstBuffer, typename SrcBuffer>
static
std::shared_ptr<DstBuffer> 
get_device_accessible_alias_impl(const std::shared_ptr<SrcBuffer> &buffer) noexcept
{
    std::shared_ptr<DstBuffer> result;

    if (buffer)
    {
        auto *alias = buffer->get_device_accessible_alias();
        if (alias)
        {
            result = std::shared_ptr<DstBuffer>(buffer, alias);
        }
    }

    return result;
}



std::shared_ptr<device_buffer> 
get_device_accessible_alias(const std::shared_ptr<host_buffer> &buffer) noexcept
{
    return get_device_accessible_alias_impl<device_buffer>(buffer);
}

std::shared_ptr<const device_buffer> 
get_device_accessible_alias(const std::shared_ptr<const host_buffer> &buffer) noexcept
{
    return get_device_accessible_alias_impl<const device_buffer>(buffer);
}

void copy(const host_buffer &src_buffer, host_buffer &dst_buffer)
{
    const auto count = require_same_buffer_size(
        src_buffer.get_size(), dst_buffer.get_size()
    );
    std::memcpy(
        dst_buffer.get_data(),
        src_buffer.get_data(),
        count
    );
}

void copy(const host_buffer &src_buffer, host_buffer &dst_buffer,
          span<const copy_region> regions )
{
    const auto* src_data = src_buffer.get_data();
    auto* dst_data = dst_buffer.get_data();
    const auto src_count = src_buffer.get_size();
    const auto dst_count = dst_buffer.get_size();
    for (const copy_region &region : regions)
    {
        require_valid_region(region, src_count, dst_count);
        std::memcpy(
            memory::offset_bytes(dst_data, region.get_destination_offset()),
            memory::offset_bytes(src_data, region.get_source_offset()),
            region.get_count()
        );
    }
}

} // namespace compute
} // namespace xmipp4
