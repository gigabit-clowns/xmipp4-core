// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/hardware/memory_heap.hpp>

#include <trompeloeil.hpp>

namespace xmipp4
{
namespace hardware
{

class mock_memory_heap
    : public memory_heap
{
public:
    MAKE_CONST_MOCK0(
        get_size,
        std::size_t(),
        noexcept override
    );

    MAKE_MOCK3(
        create_buffer, 
        std::shared_ptr<buffer>(
            std::size_t, 
            std::size_t, 
            std::unique_ptr<buffer_sentinel>
        ), 
        override
    );

};

} // namespace hardware
} // namespace xmipp4
