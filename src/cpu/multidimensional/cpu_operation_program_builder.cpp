// SPDX-License-Identifier: GPL-3.0-only

#include "cpu_operation_program_builder.hpp"

#include <xmipp4/core/multidimensional/array_signature.hpp>
#include <xmipp4/core/hardware/memory_resource.hpp>
#include <cpu/hardware/cpu_command_queue.hpp>

#include <algorithm>

namespace xmipp4 
{
namespace multidimensional
{

namespace
{
	
bool check_host_access(const array_signature &signature) noexcept
{
	const auto *resource = signature.get_memory_resource();
	if (!resource)
	{
		return false;
	}

	if (!hardware::is_host_accessible(resource->get_kind()))
	{
		return false;
	}

	return true;
}

bool check_array_signatures(
	span<const array_signature> signatures
) noexcept
{
	return std::all_of(signatures.begin(), signatures.end(), check_host_access);
}

} // anonymous namespace



backend_priority cpu_operation_program_builder::get_suitability(
	const operation& /*operation*/,
	span<const array_signature> output_signatures,
	span<const array_signature> input_signatures,
	hardware::command_queue &queue
) const
{
	if (!check_array_signatures(output_signatures))
	{
		return backend_priority::unsupported;
	}

	if (!check_array_signatures(input_signatures))
	{
		return backend_priority::unsupported;
	}

	if (!hardware::cpu_command_queue::try_cast(queue))
	{
		return backend_priority::unsupported;
	}

	return backend_priority::normal;
}

} // namespace multidimensional
} // namespace xmipp4

