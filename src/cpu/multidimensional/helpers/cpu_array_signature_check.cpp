// SPDX-License-Identifier: GPL-3.0-only

#include "cpu_array_signature_check.hpp"

#include <xmipp4/core/multidimensional/array_signature.hpp>
#include <xmipp4/core/hardware/memory_resource.hpp>

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
		return false;
	}

	return true;
}

bool cpu_check_array_signatures(
	span<const array_signature> signatures
) noexcept
{
	return std::all_of(signatures.begin(), signatures.end(), check_host_access);
}

} // namespace multidimensional
} // namespace xmipp4

