// SPDX-License-Identifier: GPL-3.0-only

/**
 * @file cpu_unified_buffer.cpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Implementation of default_cpu_unified_buffer.hpp
 * @date 2024-11-26
 * 
 */

#include <xmipp4/core/compute/cpu/cpu_unified_buffer.hpp>

namespace xmipp4
{
namespace compute
{

cpu_unified_buffer* cpu_unified_buffer::get_device_accessible_alias() noexcept
{
    return this;
}

const cpu_unified_buffer* 
cpu_unified_buffer::get_device_accessible_alias() const noexcept
{
    return this;
}

cpu_unified_buffer* cpu_unified_buffer::get_host_accessible_alias() noexcept
{
    return this;
}

const cpu_unified_buffer* 
cpu_unified_buffer::get_host_accessible_alias() const noexcept
{
    return this;
}

} // namespace compute
} // namespace xmipp4
