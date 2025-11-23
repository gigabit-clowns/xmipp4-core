// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/hardware/device_to_host_event.hpp>

#include <trompeloeil.hpp>

namespace xmipp4
{
namespace hardware
{

class mock_device_to_host_event final
	: public device_to_host_event
{
public:
	MAKE_MOCK1(signal, void(device_queue&), override);
	MAKE_CONST_MOCK0(is_signaled, bool(), override);
	MAKE_CONST_MOCK0(wait, void(), override);

};

} // namespace hardware
} // namespace xmipp4
