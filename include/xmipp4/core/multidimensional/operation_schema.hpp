// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "../platform/dynamic_shared_object.h"
#include "../span.hpp"

namespace xmipp4 
{
namespace multidimensional
{

class array_descriptor;

class operation_schema
{
public:
	XMIPP4_CORE_API
	operation_schema() noexcept;
	operation_schema(const operation_schema &other) = delete;
	operation_schema(operation_schema &&other) = delete;
	XMIPP4_CORE_API
    virtual ~operation_schema();

	operation_schema& operator=(const operation_schema &other) = delete;
	operation_schema& operator=(operation_schema &&other) = delete;

	virtual void deduce_output(
		span<array_descriptor> output_descriptors,
		span<array_descriptor> input_descriptors
	) const = 0;
};

} // namespace multidimensional
} // namespace xmipp4
