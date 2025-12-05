// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "../platform/dynamic_shared_object.h"
#include "../numerical_type.hpp"
#include "../span.hpp"

namespace xmipp4 
{
namespace multidimensional
{

class strided_layout;

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
		span<strided_layout> output_layouts,
		span<numerical_type> output_data_types,
		span<strided_layout> input_layouts,
		span<numerical_type> input_data_types
	) const = 0;
};

} // namespace multidimensional
} // namespace xmipp4
