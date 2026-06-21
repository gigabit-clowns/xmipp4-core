// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/hardware/program.hpp>

#include <trompeloeil.hpp>

namespace xmipp4
{
namespace hardware
{

class mock_program final
	: public program
{
public:
	MAKE_CONST_MOCK1(
		get_scratch_requirements,
		void(std::vector<program_scratch_requirement> &),
		override
	);
};

} // namespace hardware
} // namespace xmipp4
