// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/hardware/program.hpp>

namespace xmipp4
{
namespace hardware
{

program::program() noexcept = default;
program::~program() = default;

void program::get_scratch_requirements(
	std::vector<program_scratch_requirement> &requirements
) const
{
	requirements.clear();
}

} // namespace hardware
} // namespace xmipp4
