// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/compute/cpu/cpu_device_queue_pool.hpp>

#include <stdexcept>

namespace xmipp4
{
namespace compute
{


std::size_t cpu_device_queue_pool::get_size() const noexcept
{
    return 1;
}

cpu_device_queue& cpu_device_queue_pool::get_queue(std::size_t index)
{
    if (index > 0)
    {
        throw std::out_of_range(
            "queue index is out of range"
        );
    }

    return m_queue;
}

} // namespace compute
} // namespace xmipp4
