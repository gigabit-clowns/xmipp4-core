// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/multidimensional/data_type_policy.hpp>
#include <xmipp4/core/platform/dynamic_shared_object.h>

namespace xmipp4
{
namespace multidimensional
{

/**
 * @brief Data type policy for the abs operation.
 *
 * `infer_output` accepts arithmetic input types. For complex inputs the
 * output is set to the corresponding real (floating-point) type; for all
 * other arithmetic inputs the output matches the input type.
 *
 * `validate` enforces the same mapping.
 */
class XMIPP4_CORE_API abs_data_type_policy final
	: public data_type_policy
{
public:
	void infer_output(
		span<numerical_type> output_types,
		span<const numerical_type> input_types
	) const override;

	void validate(
		span<const numerical_type> output_types,
		span<const numerical_type> input_types
	) const override;

	static const abs_data_type_policy& get() noexcept;

private:
	static numerical_type check_input(
		span<const numerical_type> input_types,
		const char *context
	);

	static numerical_type expected_output(numerical_type input_type) noexcept;
};

} // namespace multidimensional
} // namespace xmipp4
