// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <cstddef>
#include <typeinfo>

namespace xmipp4 
{
namespace compute
{

class memory_resource;

class buffer
{
public:
    /**
     * @brief Get the size in bytes for this buffer.
     * 
     * @return std::size_t Size in bytes.
     */
    virtual std::size_t get_size() const noexcept = 0;

    /**
     * @brief Get the memory_resource where this buffer is stored. 
     * 
     * @return memory_resource& The resource where the buffer is stored.
     */
    virtual memory_resource& get_memory_resource() const noexcept = 0;

};

} // namespace compute
} // namespace xmipp4
