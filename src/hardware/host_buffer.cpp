// SPDX-License-Identifier: GPL-3.0-only

#include "host_buffer.hpp"

#include "host_memory_resource.hpp"

#include <xmipp4/core/hardware/device_queue.hpp>
#include <xmipp4/core/memory/align.hpp>
#include <xmipp4/core/memory/aligned_alloc.hpp>

namespace xmipp4
{
namespace hardware
{

host_buffer::host_buffer() noexcept
    : m_size(0)
    , m_data(nullptr)
{
}

host_buffer::host_buffer(
    void *data, 
    std::size_t size, 
    std::unique_ptr<memory_allocation_tracker> tracker
) noexcept
    : m_data(data)
    , m_size(size)
    , m_tracker(std::move(tracker))
{
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

void host_buffer::record_queue(device_queue& queue, bool exclusive)
{
    if (m_tracker)
    {
        m_tracker->record_queue(queue, exclusive);
    }
}

} // namespace hardware
} // namespace xmipp4

