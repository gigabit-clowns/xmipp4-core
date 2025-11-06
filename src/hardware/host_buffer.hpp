// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/hardware/buffer.hpp>

#include <xmipp4/core/hardware/memory_allocation_tracker.hpp>

namespace xmipp4 
{
namespace hardware
{

class host_buffer
    : public buffer
{
public:
    host_buffer() noexcept;
    host_buffer(
        void *data, 
        std::size_t size, 
        std::unique_ptr<memory_allocation_tracker> tracker
    ) noexcept;
    host_buffer(const host_buffer &other) = delete;
    host_buffer(host_buffer &&other) noexcept;
    ~host_buffer() override;

    host_buffer& operator=(const host_buffer &other) = delete;
    host_buffer& operator=(host_buffer &&other) noexcept;

    void reset() noexcept;

    void swap(host_buffer &other) noexcept;

    void* get_host_ptr() noexcept override;

    const void* get_host_ptr() const noexcept override;

    std::size_t get_size() const noexcept override;

    memory_resource& get_memory_resource() const noexcept override;

    void record_queue(device_queue &queue, bool exclusive) override;

private:
    std::size_t m_size;
    void* m_data;
    std::unique_ptr<memory_allocation_tracker> m_tracker;

};

} // namespace hardware
} // namespace xmipp4
