// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/compute/unified_memory_allocator.hpp>

#include <xmipp4/core/compute/device_queue.hpp>

#include <trompeloeil.hpp>

namespace xmipp4
{
namespace compute
{

class mock_unified_memory_allocator final
    : public unified_memory_allocator
{
public:
    MAKE_MOCK2(create_unified_buffer, std::shared_ptr<unified_buffer>(std::size_t, std::size_t), override);
    MAKE_MOCK3(create_unified_buffer, std::shared_ptr<unified_buffer>(std::size_t, std::size_t, device_queue&), override);

};

} // namespace compute
} // namespace xmipp4
