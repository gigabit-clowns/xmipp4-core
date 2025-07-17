// SPDX-License-Identifier: GPL-3.0-only

#include "default_cpu_unified_buffer.hpp"

#include <xmipp4/core/memory/aligned_alloc.hpp>

#include <stdexcept>

namespace xmipp4
{
namespace compute
{

default_cpu_unified_buffer::default_cpu_unified_buffer() noexcept
    : m_size(0)
    , m_data(nullptr)
{
}

default_cpu_unified_buffer
::default_cpu_unified_buffer(std::size_t size, std::size_t alignment)
    : m_size(size)
    , m_data(memory::aligned_alloc(memory::align_ceil(size, alignment), alignment))
{
    if(m_data == nullptr)
    {
        throw std::bad_alloc();
    }
}

default_cpu_unified_buffer
::default_cpu_unified_buffer(default_cpu_unified_buffer &&other) noexcept
    : m_size(other.m_size)
    , m_data(nullptr)
{
}

default_cpu_unified_buffer::~default_cpu_unified_buffer()
{
    reset();
}

default_cpu_unified_buffer& 
default_cpu_unified_buffer
::operator=(default_cpu_unified_buffer &&other) noexcept
{
    swap(other);
    other.reset();
    return *this;
}

void default_cpu_unified_buffer
::swap(default_cpu_unified_buffer &other) noexcept
{
    std::swap(m_size, other.m_size);
    std::swap(m_data, other.m_data);
}

void default_cpu_unified_buffer::reset() noexcept
{
    if (m_data)
    {
        memory::aligned_free(m_data);
        m_size = 0;
        m_data = nullptr;
    }
}

std::size_t default_cpu_unified_buffer::get_size() const noexcept
{
    return m_size;
}

void* default_cpu_unified_buffer::get_data() noexcept
{
    return m_data;
}

const void* default_cpu_unified_buffer::get_data() const noexcept
{
    return m_data;
}

void default_cpu_unified_buffer::record_queue(device_queue&)
{
    // No-op
}

} // namespace compute
} // namespace xmipp4
