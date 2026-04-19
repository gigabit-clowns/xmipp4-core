// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/multidimensional/operation_data_type_policy.hpp>
#include <xmipp4/core/platform/dynamic_shared_object.h>

namespace xmipp4
{
namespace multidimensional
{

/**
 * @brief Data type policy requiring all operand types to be the same.
 *
 * After validation/inference all operators will have the same data type.
 *
 * `infer_output` ensures that all inputs have the exact same data type and
 * assigns this data type to the output.
 *
 * `validate` ensures that all input/output operands have the exact same data
 * type.
 */
class XMIPP4_CORE_API homogeneous_operation_data_type_policy final
	: public operation_data_type_policy
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

	static const homogeneous_operation_data_type_policy& get() noexcept;

private:
	static void check(numerical_type type);
};

} // namespace multidimensional
} // namespace xmipp4
