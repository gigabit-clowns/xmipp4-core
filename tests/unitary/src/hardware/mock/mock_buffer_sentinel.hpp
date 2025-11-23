// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/hardware/buffer_sentinel.hpp>

#include <trompeloeil.hpp>

namespace xmipp4
{
namespace hardware
{

class mock_buffer_sentinel final
	: public buffer_sentinel
{
public:
	MAKE_MOCK2(
		record_queue,
		void(device_queue&, bool),
		override
	);

};

} // namespace hardware
} // namespace xmipp4
