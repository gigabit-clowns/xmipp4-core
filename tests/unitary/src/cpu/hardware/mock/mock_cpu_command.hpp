// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/cpu/hardware/cpu_command.hpp>

#include <trompeloeil.hpp>

namespace xmipp4
{
namespace hardware
{

class mock_cpu_command final
	: public cpu_command
{
public:
	MAKE_CONST_MOCK3(
		execute,
		void(
			span<void* const> outputs,
			span<const void* const> inputs,
			void *scratch
		),
		override
	);
};

} // namespace hardware
} // namespace xmipp4
