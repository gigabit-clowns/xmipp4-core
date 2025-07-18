// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/compute/unified_buffer.hpp>

namespace xmipp4 
{
namespace compute
{

/**
 * @brief Implementation of unified_buffer with malloc.
 * 
 */
class cpu_unified_buffer final
    : public unified_buffer
{
public:
    cpu_unified_buffer() noexcept;
    cpu_unified_buffer(std::size_t size, std::size_t alignment);
    cpu_unified_buffer(const cpu_unified_buffer &other) = delete;
    cpu_unified_buffer(cpu_unified_buffer &&other) noexcept;
    ~cpu_unified_buffer() override;

    cpu_unified_buffer& 
    operator=(const cpu_unified_buffer &other) = delete;
    cpu_unified_buffer& 
    operator=(cpu_unified_buffer &&other) noexcept;

    void swap(cpu_unified_buffer &other) noexcept;
    void reset() noexcept;

    std::size_t get_size() const noexcept override;

    void* get_data() noexcept override;
    const void* get_data() const noexcept override;

    void record_queue(device_queue &queue) override;

private:
    std::size_t m_size;
    void* m_data;

}; 

} // namespace compute
} // namespace xmipp4
