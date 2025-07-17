// SPDX-License-Identifier: GPL-3.0-only

/**
 * @file host_buffer.cpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Implementation of host_buffer.hpp
 * @date 2024-12-08
 * 
 */

#include <xmipp4/core/compute/device_buffer.hpp>

namespace xmipp4
{
namespace compute
{

template <typename DstBuffer, typename SrcBuffer>
static
std::shared_ptr<DstBuffer> 
get_host_accessible_alias_impl(const std::shared_ptr<SrcBuffer> &buffer) noexcept
{
    std::shared_ptr<DstBuffer> result;

    if (buffer)
    {
        auto *alias = buffer->get_host_accessible_alias();
        if (alias)
        {
            result = std::shared_ptr<DstBuffer>(buffer, alias);
        }
    }

    return result;
}



std::shared_ptr<host_buffer> 
get_host_accessible_alias(const std::shared_ptr<device_buffer> &buffer) noexcept
{
    return get_host_accessible_alias_impl<host_buffer>(buffer);
}

std::shared_ptr<const host_buffer> 
get_host_accessible_alias(const std::shared_ptr<const device_buffer> &buffer) noexcept
{
    return get_host_accessible_alias_impl<const host_buffer>(buffer);
}

} // namespace compute
} // namespace xmipp4
