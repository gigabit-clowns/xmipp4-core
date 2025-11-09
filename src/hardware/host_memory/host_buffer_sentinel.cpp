// SPDX-License-Identifier: GPL-3.0-only

#include "host_buffer_sentinel.hpp"

#include <xmipp4/core/memory/aligned_alloc.hpp>

#include <stdexcept>

namespace xmipp4
{
namespace hardware
{

host_buffer_sentinel::host_buffer_sentinel(
    std::size_t size, 
    std::size_t alignment
)
    : m_data(memory::aligned_alloc(size, alignment))
{
    if (size > 0 && m_data == nullptr)
    {
        throw std::bad_alloc();
    }
}

host_buffer_sentinel::~host_buffer_sentinel()
{
    memory::aligned_free(m_data);
}

void* host_buffer_sentinel::get_data() noexcept
{
    return m_data;
}

void host_buffer_sentinel::record_queue(device_queue&, bool)
{
    throw std::invalid_argument(
        "host_buffer_sentinel does not support queue execution"
    );
}

} // namespace hardware
} // namespace xmipp4
