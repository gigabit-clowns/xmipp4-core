// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "../unified_memory_allocator.hpp"

namespace xmipp4 
{
namespace compute
{

class cpu_unified_buffer;

class cpu_unified_memory_allocator final
    : public unified_memory_allocator
{
public:
    cpu_unified_memory_allocator() = default;
    cpu_unified_memory_allocator(const cpu_unified_memory_allocator &other) = default;
    cpu_unified_memory_allocator(cpu_unified_memory_allocator &&other) = default;
    ~cpu_unified_memory_allocator() override = default;

    cpu_unified_memory_allocator& 
    operator=(const cpu_unified_memory_allocator &other) = default;
    cpu_unified_memory_allocator& 
    operator=(cpu_unified_memory_allocator &&other) = default;

    std::shared_ptr<cpu_unified_buffer> 
    create_unified_buffer_impl(std::size_t size, std::size_t alignment);

    std::shared_ptr<unified_buffer> 
    create_unified_buffer(std::size_t size, std::size_t alignment) override;

    std::shared_ptr<unified_buffer> 
    create_unified_buffer(std::size_t size, 
                          std::size_t alignment, 
                          device_queue &queue) override;

}; 

} // namespace compute
} // namespace xmipp4

