// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/compute/device_buffer.hpp>

#include <trompeloeil.hpp>

namespace xmipp4
{
namespace compute
{

class mock_device_buffer final
    : public device_buffer
{
public:
    MAKE_MOCK0(get_size, std::size_t (), const noexcept override);
    MAKE_MOCK0(get_host_accessible_alias, host_buffer* (), noexcept override);
    MAKE_CONST_MOCK0(get_host_accessible_alias, const host_buffer* (), noexcept override);
    MAKE_MOCK1(record_queue, void (device_queue&), override);

};
    
} // namespace compute
} // namespace xmipp4
