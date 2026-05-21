// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/hardware/command.hpp>

namespace xmipp4
{
namespace hardware
{

command::command() noexcept = default;
command::~command() = default;

std::size_t command::get_required_scratch_size() const noexcept
{
	return 0UL;
}

} // namespace hardware
} // namespace xmipp4
