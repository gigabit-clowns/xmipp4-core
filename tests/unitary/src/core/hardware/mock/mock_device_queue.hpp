// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/hardware/device_queue.hpp>

#include <trompeloeil.hpp>

namespace xmipp4
{
namespace hardware
{

class mock_device_queue final
	: public device_queue
{
public:
	MAKE_MOCK0(wait_until_completed, void(), const override);
	MAKE_MOCK0(is_idle, bool(), const noexcept override);
};

} // namespace hardware
} // namespace xmipp4
