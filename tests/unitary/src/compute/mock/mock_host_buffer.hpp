// SPDX-License-Identifier: GPL-3.0-only

#pragma once

/**
 * @file mock_host_buffer.cpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Mock for host_buffer interface.
 * @date 2024-12-10
 */

#include <xmipp4/core/compute/host_buffer.hpp>

#include <trompeloeil.hpp>

namespace xmipp4
{
namespace compute
{

class mock_host_buffer final
    : public host_buffer
{
public:
    MAKE_MOCK0(get_size, std::size_t (), const noexcept override);
    MAKE_MOCK0(get_data, void* (), noexcept override);
    MAKE_CONST_MOCK0(get_data, const void* (), noexcept override);
    MAKE_MOCK0(get_device_accessible_alias, device_buffer* (), noexcept override);
    MAKE_CONST_MOCK0(get_device_accessible_alias, const device_buffer* (), noexcept override);
    MAKE_MOCK1(record_queue, void (device_queue&), override);

};

} // namespace compute
} // namespace xmipp4
