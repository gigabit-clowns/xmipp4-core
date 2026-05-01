// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <cstddef>
#include <cstdint>

extern "C"
{

std::int32_t
xmipp4_rust_promote_types(std::int32_t type1, std::int32_t type2) noexcept;

std::size_t
xmipp4_rust_compute_storage_requirement(
	const std::size_t *extents,
	const std::ptrdiff_t *strides,
	std::size_t rank,
	std::ptrdiff_t offset
) noexcept;

} // extern "C"
