// SPDX-License-Identifier: GPL-3.0-only

#include "host_buffer.hpp"

#include "host_memory_resource.hpp"

#include <xmipp4/core/compute/device_queue.hpp>
#include <xmipp4/core/memory/align.hpp>
#include <xmipp4/core/memory/aligned_alloc.hpp>

namespace xmipp4
{
namespace compute
{

host_buffer::host_buffer() noexcept
    : m_size(0)
    , m_data(nullptr)
{
}

host_buffer::host_buffer(std::size_t size, std::size_t alignment)
    : m_size(memory::align_ceil(size, alignment))
    , m_data(memory::aligned_alloc(m_size, alignment))
{
    if(m_data == nullptr && m_size != 0)
    {
        throw std::bad_alloc();
    }
}

host_buffer::host_buffer(host_buffer &&other) noexcept
    : host_buffer()
{
    swap(other);
}

host_buffer::~host_buffer()
{
    reset();
}

host_buffer& host_buffer::operator=(host_buffer &&other) noexcept
{
    if (this != &other)
    {
        reset();
        swap(other);
    }

    return *this;
}

void host_buffer::reset() noexcept
{
    if (m_data)
    {
        memory::aligned_free(m_data);
        m_data = nullptr;
        m_size = 0;
    }
}

void host_buffer::swap(host_buffer &other) noexcept
{
    std::swap(m_size, other.m_size);
    std::swap(m_data, other.m_data);
}

void* host_buffer::get_host_ptr() noexcept
{
    return m_data;
}

const void* host_buffer::get_host_ptr() const noexcept
{
    return m_data;
}

std::size_t host_buffer::get_size() const noexcept
{
    return m_size;
}

memory_resource& host_buffer::get_memory_resource() const noexcept
{
    return host_memory_resource::get();
}

void host_buffer::record_queue(device_queue& queue, bool)
{
    queue.wait_until_completed(); // Synchronous operation
}

} // namespace compute
} // namespace xmipp4

