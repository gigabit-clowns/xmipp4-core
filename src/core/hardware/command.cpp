// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/hardware/command.hpp>

namespace xmipp4
{
namespace hardware
{

command::command() noexcept = default;
command::~command() = default;

bool command::get_scratch_requirement(
	std::size_t& /*size*/,
	std::size_t& /*alignment*/
) const noexcept
{
	return false;
}

} // namespace hardware
} // namespace xmipp4
