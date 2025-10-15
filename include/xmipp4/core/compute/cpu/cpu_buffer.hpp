// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "../buffer.hpp"

namespace xmipp4 
{
namespace compute
{

class cpu_buffer final
    : public buffer
{
public:
    cpu_buffer(std::size_t size, std::size_t alignment);
    cpu_buffer(const cpu_buffer &other) = delete;
    cpu_buffer(cpu_buffer &&other);
    virtual ~cpu_buffer() override;

    cpu_buffer& operator=(const cpu_buffer &other) = delete;
    cpu_buffer& operator=(cpu_buffer &&other);

    void* get_host_ptr() noexcept override;

    const void* get_host_ptr() const noexcept override;

    std::size_t get_size() const noexcept override;

    memory_resource& get_memory_resource() const noexcept override;

private:
    void* m_data;
    std::size_t m_size;

}; 

} // namespace compute
} // namespace xmipp4
