// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/hardware/command.hpp>

#include <trompeloeil.hpp>

namespace xmipp4
{
namespace hardware
{

class mock_command final
	: public command
{
public:
	MAKE_CONST_MOCK0(
		get_scratch_requirements,
		span<const command_scratch_requirement>(),
		override
	);
};

} // namespace hardware
} // namespace xmipp4
