// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/hardware/buffer_sentinel.hpp>

namespace xmipp4 
{
namespace hardware
{

/**
 * @brief Implementation of buffer_sentinel based on malloc/free
 * 
 */
class host_buffer_sentinel final
    : public buffer_sentinel
{
public:
    host_buffer_sentinel(std::size_t size, std::size_t alignment);
    ~host_buffer_sentinel() override;

    std::size_t get_size() const noexcept;
    void* get_data() noexcept;

    void record_queue(device_queue &queue, bool exclusive) override;

private:
    std::size_t m_size;
    void *m_data;

};

} // namespace hardware
} // namespace xmipp4
