// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/cpu/hardware/cpu_program.hpp>

#include <trompeloeil.hpp>

namespace xmipp4
{
namespace hardware
{

class mock_cpu_program final
	: public cpu_program
{
public:
	MAKE_CONST_MOCK3(
		execute,
		void(
			span<const std::shared_ptr<buffer>> outputs,
			span<const std::shared_ptr<const buffer>> inputs,
			span<const std::shared_ptr<buffer>> scratch
		),
		override
	);
};

} // namespace hardware
} // namespace xmipp4
