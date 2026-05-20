// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/hardware/command_queue.hpp>

#include <trompeloeil.hpp>

namespace xmipp4
{
namespace hardware
{

class mock_command_queue final
	: public command_queue
{
public:
	MAKE_MOCK3(
		submit,
		void(
			const command &command,
			span<const std::shared_ptr<buffer>> output_operands,
			span<const std::shared_ptr<const buffer>> input_operands
		),
		override
	);
	MAKE_MOCK1(signal, void(event &event), override);
	MAKE_MOCK1(wait, void(const event &event), override);
	MAKE_CONST_MOCK0(wait_until_completed, void(), override);
	MAKE_CONST_MOCK0(is_idle, bool(), override);
};

} // namespace hardware
} // namespace xmipp4
