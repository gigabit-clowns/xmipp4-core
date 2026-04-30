// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/multidimensional/operation_data_type_policy.hpp>
#include <xmipp4/core/platform/dynamic_shared_object.h>

namespace xmipp4
{
namespace multidimensional
{

/**
 * @brief Data type policy requiring all operand types to be boolean
 *
 * After successful validation/inference all operators will be boolean.
 *
 * `deduce_output` ensures that all inputs are boolean type and sets the
 * output to boolean.
 *
 * `validate` ensures that all input/output operands are boolean.
 */
class XMIPP4_CORE_API boolean_operation_data_type_policy final
	: public operation_data_type_policy
{
public:
	void deduce_output(
		span<numerical_type> output_types,
		span<const numerical_type> input_types
	) const override;

	void validate(
		span<const numerical_type> output_types,
		span<const numerical_type> input_types
	) const override;

	static const boolean_operation_data_type_policy& get() noexcept;
};

} // namespace multidimensional
} // namespace xmipp4
