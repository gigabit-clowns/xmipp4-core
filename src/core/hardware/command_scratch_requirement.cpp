// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/hardware/command_scratch_requirement.hpp>

namespace xmipp4
{
namespace hardware
{

command_scratch_requirement::command_scratch_requirement(
	std::size_t size,
	std::size_t alignment,
	memory_resource_affinity affinity
) noexcept
	: m_size(size)
	, m_alignment(alignment)
	, m_affinity(affinity)
{
}

std::size_t command_scratch_requirement::get_size() const noexcept
{
	return m_size;
}

std::size_t command_scratch_requirement::get_alignment() const noexcept
{
	return m_alignment;
}

memory_resource_affinity command_scratch_requirement::get_affinity() const
	noexcept
{
	return m_affinity;
}

} // namespace hardware
} // namespace xmipp4
