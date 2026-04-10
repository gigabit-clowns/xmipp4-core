// SPDX-License-Identifier: GPL-3.0-only

#include "cpu_array_signature_check.hpp"

#include <xmipp4/core/multidimensional/array_signature.hpp>

#include <algorithm>

namespace xmipp4 
{
namespace multidimensional
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
		return true;
	}
}

bool cpu_check_array_signatures(span<const array_signature> signatures) noexcept
{
	return std::all_of(signatures.begin(), signatures.end(), check_host_access);
}

bool cpu_check_array_signatures(
	span<const array_signature> output_signatures,
	span<const array_signature> input_signatures
) noexcept
{
	return 
		cpu_check_array_signatures(output_signatures) &&
		cpu_check_array_signatures(input_signatures) ;
}

} // namespace multidimensional
} // namespace xmipp4
