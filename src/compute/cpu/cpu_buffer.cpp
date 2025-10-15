// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/compute/cpu/cpu_buffer.hpp>

namespace xmipp4
{
namespace compute
{

cpu_buffer::cpu_buffer(std::size_t size, std::size_t alignment)
{
    // TODO
}

cpu_buffer::cpu_buffer(cpu_buffer &&other)
{
    // TODO
}

cpu_buffer::~cpu_buffer()
{
    // TODO
}

cpu_buffer& cpu_buffer::operator=(cpu_buffer &&other)
{
    // TODO
}

void* cpu_buffer::get_host_ptr() noexcept
{
    return m_data;
}

const void* cpu_buffer::get_host_ptr() const noexcept
{
    return m_data;
}

std::size_t cpu_buffer::get_size() const noexcept
{
    return m_size;
}

memory_resource& cpu_buffer::get_memory_resource() const noexcept
{
    // TODO
}

} // namespace compute
} // namespace xmipp4
