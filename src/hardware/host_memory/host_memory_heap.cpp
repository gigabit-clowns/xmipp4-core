// SPDX-License-Identifier: GPL-3.0-only

#include "host_memory_heap.hpp"

#include <xmipp4/core/hardware/buffer.hpp>
#include <xmipp4/core/memory/align.hpp>
#include <xmipp4/core/memory/aligned_alloc.hpp>

#include "host_memory_resource.hpp"

namespace xmipp4
{
namespace hardware
{

host_memory_heap::host_memory_heap() noexcept
    : m_size(0)
    , m_data(nullptr)
{
}

host_memory_heap::host_memory_heap(
    std::size_t size,
    std::size_t alignment
)
    : m_size(memory::align_ceil(size, alignment))
    , m_data(memory::aligned_alloc(m_size, alignment))
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
        memory::aligned_free(m_data);
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
    std::unique_ptr<buffer_sentinel> sentinel
)
{
    if (offset + size > m_size)
    {
        throw std::out_of_range("Requested allocation exceeds heap size");
    }

    return std::make_shared<buffer>(
        memory::offset_bytes(m_data, offset),
        size, 
        host_memory_resource::get(),
        std::move(sentinel)
    );
}

} // namespace hardware
} // namespace xmipp4
