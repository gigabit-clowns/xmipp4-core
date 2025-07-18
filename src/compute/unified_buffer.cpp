// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/compute/unified_buffer.hpp>

namespace xmipp4
{
namespace compute
{

unified_buffer* unified_buffer::get_device_accessible_alias() noexcept
{
    return this;
}

const unified_buffer* 
unified_buffer::get_device_accessible_alias() const noexcept
{
    return this;
}

unified_buffer* unified_buffer::get_host_accessible_alias() noexcept
{
    return this;
}

const unified_buffer* 
unified_buffer::get_host_accessible_alias() const noexcept
{
    return this;
}

} // namespace compute
} // namespace xmipp4
