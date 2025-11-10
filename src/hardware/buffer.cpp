// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/hardware/buffer.hpp>

#include <xmipp4/core/exceptions/invalid_operation_error.hpp>

#include <iostream>

namespace xmipp4
{
namespace hardware
{

buffer::buffer(
    void *host_pointer,
    std::size_t size,
    std::reference_wrapper<memory_resource> resource,
    std::unique_ptr<buffer_sentinel> sentinel
)
    : m_host_pointer(host_pointer)
    , m_size(size)
    , m_memory_resource(resource)
    , m_sentinel(std::move(sentinel))
{
}

void* buffer::get_host_ptr() noexcept
{
    return m_host_pointer;
}

const void* buffer::get_host_ptr() const noexcept
{
    return m_host_pointer;
}

std::size_t buffer::get_size() const noexcept
{
    return m_size;
}

memory_resource& buffer::get_memory_resource() const noexcept
{
    return m_memory_resource;
}

void buffer::record_queue(device_queue& queue, bool exclusive)
{
    if (!m_sentinel)
    {
        throw invalid_operation_error(
            "Can not call buffer::record_queue on a buffer without a sentinel"
        );
    }

    m_sentinel->record_queue(queue, exclusive);
}

} // namespace hardware
} // namespace xmipp4

