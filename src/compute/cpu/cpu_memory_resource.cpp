// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/compute/cpu/cpu_memory_resource.hpp>

#include <xmipp4/core/compute/cpu/cpu_memory_allocator.hpp>

namespace xmipp4
{
namespace compute
{

memory_resource_kind cpu_memory_resource::get_kind() const noexcept
{
    return memory_resource_kind::unified;
}

std::shared_ptr<memory_allocator> cpu_memory_resource::create_allocator()
{
    return std::make_shared<cpu_memory_allocator>();
}

} // namespace compute
} // namespace xmipp4
