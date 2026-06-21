// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/hardware/command.hpp>

namespace xmipp4
{
namespace hardware
{

command::command() noexcept = default;
command::~command() = default;

span<const command_scratch_requirement> 
command::get_scratch_requirements() const noexcept
{
	return {};
}

} // namespace hardware
} // namespace xmipp4
