// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/multidimensional/operation_data_type_policy.hpp>
#include <xmipp4/core/platform/dynamic_shared_object.h>

namespace xmipp4
{
namespace multidimensional
{

/**
 * @brief Data type policy for the abs operation.
 * 
 * The input may be any arithmetic numerical type. For non-complex types, the
 * output is the same as the input type. For complex types, the output must
 * be the real equivalent of the input type (e.g. complex_float32 -> float32).
 */
class XMIPP4_CORE_API abs_operation_data_type_policy final
	: public operation_data_type_policy
{
public:
	void deduce(
		span<numerical_type> canonical_output_types,
		span<const numerical_type> input_types
	) const override;

	static const abs_operation_data_type_policy& get() noexcept;

private:
	static numerical_type check_input(
		span<const numerical_type> input_types,
		const char *context
	);

	static numerical_type expected_output(numerical_type input_type) noexcept;
};

} // namespace multidimensional
} // namespace xmipp4
