// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "platform/dynamic_shared_object.h"

#include <ostream>

namespace xmipp4
{

/**
 * @brief Representation of all considered numerical types for
 * computations.
 * 
 */
enum class numerical_type
{
	unknown = -1,

	boolean,

	char8,

	int8,
	uint8,
	int16,
	uint16,
	int32,
	uint32,
	int64,
	uint64,

	float16,
	float32,
	float64,

	complex_float16,
	complex_float32,
	complex_float64,

	// Add here

	count
};

/**
 * @brief Get the size of a numerical type.
 * 
 * @param type The type for which the size should be obtained.
 * @return std::size_t The size.
 */
XMIPP4_CORE_API
std::size_t get_size(numerical_type type) noexcept;

/**
 * @brief Create a complex version, if exists.
 * 
 * Create the complex version of a numerical type, if this exists.
 * Otherwise unknown is returned. This includes an already complex type.
 * 
 * @param type 
 * @return numerical_type.
 */
XMIPP4_CORE_API
numerical_type make_complex(numerical_type type) noexcept;

/**
 * @brief Get the common type of two numerical types.
 * 
 * @param type1 First numerical type.
 * @param type2 Second numerical type.
 * @return numerical_type The common type.
 */
XMIPP4_CORE_API
numerical_type 
promote_types(numerical_type type1, numerical_type type2) noexcept;

XMIPP4_CORE_API
const char* to_string(numerical_type type) noexcept;

XMIPP4_CORE_API
std::ostream& operator<<(std::ostream &os, numerical_type type);

} // namespace xmipp4
