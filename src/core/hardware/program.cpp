// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/hardware/program.hpp>

namespace xmipp4
{

program::program() noexcept = default;
program::~program() = default;

span<const program_scratch_requirement>
program::get_scratch_requirements() const
{
	return {};
}

} // namespace xmipp4
