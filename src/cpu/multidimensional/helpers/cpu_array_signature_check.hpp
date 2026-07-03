// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/span.hpp>

namespace xmipp4
{
namespace multidimensional
{

class array_signature;

bool check_host_access(const array_signature &signature) noexcept;

bool cpu_check_array_signatures(
	span<const array_signature> signatures
) noexcept;

} // namespace multidimensional
} // namespace xmipp4

