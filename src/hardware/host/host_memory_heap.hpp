// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/hardware/memory_heap.hpp>

namespace xmipp4 
{
namespace hardware
{

class host_memory_heap
    : public memory_heap
{
public:
    host_memory_heap() noexcept;
    host_memory_heap(std::size_t size);
    host_memory_heap(const host_memory_heap &other) = delete;
    host_memory_heap(host_memory_heap &&other) noexcept;
    ~host_memory_heap() override;

    host_memory_heap& operator=(const host_memory_heap &other) = delete;
    host_memory_heap& operator=(host_memory_heap &&other) noexcept;

    void reset() noexcept;
    void swap(host_memory_heap &other) noexcept;

    std::size_t get_size() const noexcept override;

    std::shared_ptr<buffer> create_buffer(
        std::size_t offset, 
        std::size_t size,
        std::unique_ptr<memory_sentinel> sentinel
    ) override;

private:
    std::size_t m_size;
    void *m_data;

};

} // namespace hardware
} // namespace xmipp4
