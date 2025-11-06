// SPDX-License-Identifier: GPL-3.0-only

#include "host_memory_heap.hpp"

#include "host_buffer.hpp"

#include <cstdlib>

namespace xmipp4
{
namespace hardware
{

host_memory_heap::host_memory_heap() noexcept
    : m_size(0)
    , m_data(nullptr)
{
}

host_memory_heap::host_memory_heap(std::size_t size)
    : m_size(size)
    , m_data(std::malloc(size))
{
    if(m_data == nullptr && m_size != 0)
    {
        throw std::bad_alloc();
    }
}

host_memory_heap::host_memory_heap(host_memory_heap &&other) noexcept
    : host_memory_heap()
{
    swap(other);
}

host_memory_heap::~host_memory_heap()
{
    reset();
}

host_memory_heap& host_memory_heap::operator=(host_memory_heap &&other) noexcept
{
    if (this != &other)
    {
        reset();
        swap(other);
    }

    return *this;
}

void host_memory_heap::reset() noexcept
{
    if (m_data)
    {
        std::free(m_data);
        m_data = nullptr;
        m_size = 0;
    }
}

void host_memory_heap::swap(host_memory_heap &other) noexcept
{
    std::swap(m_size, other.m_size);
    std::swap(m_data, other.m_data);
}

std::size_t host_memory_heap::get_size() const noexcept
{
    return m_size;
}

std::shared_ptr<buffer> host_memory_heap::create_buffer(
    std::size_t offset, 
    std::size_t size,
    std::unique_ptr<memory_allocation_tracker> tracker
)
{
    return std::make_shared<host_buffer>(offset, size, std::move(tracker));
}

} // namespace hardware
} // namespace xmipp4
