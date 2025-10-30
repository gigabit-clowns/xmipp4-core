// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/compute/buffer.hpp>

#include <trompeloeil.hpp>

namespace xmipp4
{
namespace compute
{

class mock_buffer final
    : public buffer
{
public:
    MAKE_MOCK0(get_host_ptr, void*(), noexcept, override);
    MAKE_CONST_MOCK0(get_host_ptr, const void*(), noexcept, override);
    MAKE_CONST_MOCK0(get_size, std::size_t(), noexcept, override);
    MAKE_CONST_MOCK0(get_memory_resource, memory_resource&(), noexcept, override);
    MAKE_MOCK2(record_queue, void(device_queue&, bool), override);

};

} // namespace compute
} // namespace xmipp4
