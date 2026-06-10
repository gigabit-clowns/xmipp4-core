// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/hardware/command.hpp>

namespace xmipp4
{
namespace hardware
{

command::command() noexcept = default;
command::~command() = default;

void command::get_scratch_requirements(
	std::vector<command_scratch_requirement> &requirements
) const
{
	requirements = {};
}

} // namespace hardware
} // namespace xmipp4
