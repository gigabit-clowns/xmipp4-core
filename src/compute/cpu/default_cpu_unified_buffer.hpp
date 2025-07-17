// SPDX-License-Identifier: GPL-3.0-only

#pragma once

/**
 * @file default_cpu_unified_buffer.hpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Defines the compute::default_cpu_unified_buffer interface
 * @date 2024-10-29
 * 
 */

#include <xmipp4/core/compute/cpu/cpu_unified_buffer.hpp>

namespace xmipp4 
{
namespace compute
{

/**
 * @brief Special implementation of device_buffer that allows to 
 * allocate memory in the host as if it were a device.
 * 
 */
class default_cpu_unified_buffer final
    : public cpu_unified_buffer
{
public:
    default_cpu_unified_buffer() noexcept;
    default_cpu_unified_buffer(std::size_t size, std::size_t alignment);
    default_cpu_unified_buffer(const default_cpu_unified_buffer &other) = delete;
    default_cpu_unified_buffer(default_cpu_unified_buffer &&other) noexcept;
    ~default_cpu_unified_buffer() override;

    default_cpu_unified_buffer& 
    operator=(const default_cpu_unified_buffer &other) = delete;
    default_cpu_unified_buffer& 
    operator=(default_cpu_unified_buffer &&other) noexcept;

    void swap(default_cpu_unified_buffer &other) noexcept;
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
