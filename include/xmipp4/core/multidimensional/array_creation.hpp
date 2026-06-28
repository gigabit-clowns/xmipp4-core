// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "array.hpp"
#include "array_view.hpp"

#include <xmipp4/core/scalar_value.hpp>
#include <xmipp4/core/hardware/memory_resource_affinity.hpp>

namespace xmipp4 
{
namespace multidimensional
{

class execution_context;

XMIPP4_CORE_API
array empty(
	array_descriptor descriptor, 
	hardware::memory_resource_affinity affinity,
	const execution_context &context,
	array *out = nullptr
);

XMIPP4_CORE_API
array zeros(
	array_descriptor descriptor, 
	hardware::memory_resource_affinity affinity,
	const execution_context &context,
	array *out = nullptr
);

XMIPP4_CORE_API
array ones(
	array_descriptor descriptor, 
	hardware::memory_resource_affinity affinity,
	const execution_context &context,
	array *out = nullptr
);

XMIPP4_CORE_API
array full(
	array_descriptor descriptor, 
	hardware::memory_resource_affinity affinity,
	const scalar_value &fill_value,
	const execution_context &context,
	array *out = nullptr
);

XMIPP4_CORE_API
array copy(
	array_view source,
	const execution_context &context,
	array *out = nullptr
);

XMIPP4_CORE_API
array fill(
	array &out,
	const scalar_value &fill_value,
	const execution_context &context
);

} // namespace multidimensional
} // namespace xmipp4
