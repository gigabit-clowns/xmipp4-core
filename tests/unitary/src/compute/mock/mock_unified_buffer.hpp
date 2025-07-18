// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/compute/unified_buffer.hpp>

#include <xmipp4/core/compute/device_queue.hpp>

#include <trompeloeil.hpp>

namespace xmipp4
{
namespace compute
{

class mock_unified_buffer final
    : public unified_buffer
{
public:
    MAKE_MOCK0(get_size, std::size_t (), const noexcept override);
    MAKE_MOCK0(get_data, void* (), noexcept override);
    MAKE_CONST_MOCK0(get_data, const void* (), noexcept override);
    MAKE_MOCK1(record_queue, void (device_queue&), override);

};

} // namespace compute
} // namespace xmipp4
