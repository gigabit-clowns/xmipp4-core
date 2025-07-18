// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/compute/cpu/cpu_unified_buffer.hpp>

#include <xmipp4/core/memory/align.hpp>
#include <xmipp4/core/memory/aligned_alloc.hpp>
#include <xmipp4/core/compute/cpu/cpu_device_queue.hpp>

#include <stdexcept>

namespace xmipp4
{
namespace compute
{

cpu_unified_buffer::cpu_unified_buffer() noexcept
    : m_size(0)
    , m_data(nullptr)
{
}

cpu_unified_buffer
::cpu_unified_buffer(std::size_t size, std::size_t alignment)
    : m_size(size)
    , m_data(memory::aligned_alloc(memory::align_ceil(size, alignment), alignment))
{
    if(m_data == nullptr)
    {
        throw std::bad_alloc();
    }
}

cpu_unified_buffer
::cpu_unified_buffer(cpu_unified_buffer &&other) noexcept
    : m_size(other.m_size)
    , m_data(nullptr)
{
}

cpu_unified_buffer::~cpu_unified_buffer()
{
    reset();
}

cpu_unified_buffer& 
cpu_unified_buffer
::operator=(cpu_unified_buffer &&other) noexcept
{
    swap(other);
    other.reset();
    return *this;
}

void cpu_unified_buffer
::swap(cpu_unified_buffer &other) noexcept
{
    std::swap(m_size, other.m_size);
    std::swap(m_data, other.m_data);
}

void cpu_unified_buffer::reset() noexcept
{
    if (m_data)
    {
        memory::aligned_free(m_data);
        m_size = 0;
        m_data = nullptr;
    }
}

std::size_t cpu_unified_buffer::get_size() const noexcept
{
    return m_size;
}

void* cpu_unified_buffer::get_data() noexcept
{
    return m_data;
}

const void* cpu_unified_buffer::get_data() const noexcept
{
    return m_data;
}

void cpu_unified_buffer::record_queue(device_queue& queue)
{
    dynamic_cast<cpu_device_queue&>(queue); // Check
    // No-op
}

} // namespace compute
} // namespace xmipp4
