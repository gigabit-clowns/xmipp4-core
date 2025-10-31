// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/compute/memory_resource_kind.hpp>

namespace xmipp4 
{
namespace compute
{

bool is_host_accessible(memory_resource_kind kind) noexcept
{
    switch (kind)
    {
    case memory_resource_kind::unified:
    case memory_resource_kind::managed:
    case memory_resource_kind::device_mapped:
    case memory_resource_kind::host_staging:
    case memory_resource_kind::host:
        return true;

    default:
        return false;

    }
}

bool is_device_accessible(memory_resource_kind kind) noexcept
{
    switch (kind)
    {
    case memory_resource_kind::unified:
    case memory_resource_kind::managed:
    case memory_resource_kind::device_local:
    case memory_resource_kind::device_mapped:
        return true;

    default:
        return false;

    }
}

} // namespace compute
} // namespace xmipp4
