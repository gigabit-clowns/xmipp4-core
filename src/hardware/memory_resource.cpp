// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/hardware/memory_resource.hpp>

#include "host_memory_resource.hpp"

namespace xmipp4
{
namespace hardware
{

memory_resource& get_host_memory_resource() noexcept
{
    return host_memory_resource::get();
}

} // namespace hardware
} // namespace xmipp4
