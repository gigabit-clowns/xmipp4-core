// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/multidimensional/operation.hpp>
#include <xmipp4/core/platform/dynamic_shared_object.h>

namespace xmipp4 
{
namespace multidimensional
{

/**
 * @brief Data type policy requiring all operand types to be the same.
 * 
 * After validation/inference all operators will have tha same data type.
 * 
 * `infer_output` ensures that all inputs have the exact same data type and 
 * assigns this data type to the output.
 * 
 * `validate` ensures that all input/output operands have the exact same data
 * type.
 */
class XMIPP4_CORE_API same_data_type_policy final
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

	const same_data_type_policy& get() noexcept;
};

/**
 * @brief Data type policy requiring all operand types to be the same floating 
 * point data type.
 * 
 * After successful validation/inference all operators will have tha same 
 * floating point data type.
 * 
 * `infer_output` ensures that all inputs have the exact same floating point 
 * data type and assigns this data type to the output.
 * 
 * `validate` ensures that all input/output operands have the exact same 
 * floating point data type.
 */
class XMIPP4_CORE_API same_floating_data_type_policy final
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

	const same_floating_data_type_policy& get() noexcept;
};

/**
 * @brief Data type policy requiring all operand types to be the same complex
 * data type.
 * 
 * After successful validation/inference all operators will have tha same 
 * complex data type.
 * 
 * `infer_output` ensures that all inputs have the exact same complex data type 
 * and assigns this data type to the output.
 * 
 * `validate` ensures that all input/output operands have the exact same complex
 * data type.
 */
class XMIPP4_CORE_API same_complex_data_type_policy final
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

	const same_complex_data_type_policy& get() noexcept;
};

/**
 * @brief Data type policy requiring all operand types to be the same arithmetic
 * data type.
 * 
 * After successful validation/inference all operators will have tha same 
 * arithmetic data type.
 * 
 * `infer_output` ensures that all inputs have the exact same arithmetic data 
 * type and assigns this data type to the output.
 * 
 * `validate` ensures that all input/output operands have the exact same
 * arithmetic data type.
 */
class XMIPP4_CORE_API same_arithmetic_data_type_policy final
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

	const same_arithmetic_data_type_policy& get() noexcept;
};

/**
 * @brief Data type policy requiring all operand types to be the same integer
 * data type.
 * 
 * After successful validation/inference all operators will have tha same 
 * integer data type.
 * 
 * `infer_output` ensures that all inputs have the exact same integer data 
 * type and assigns this data type to the output.
 * 
 * `validate` ensures that all input/output operands have the exact same
 * integer data type.
 */
class XMIPP4_CORE_API same_integer_data_type_policy final
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

	const same_integer_data_type_policy& get() noexcept;
};

/**
 * @brief Data type policy requiring all operand types to be boolean
 * 
 * After successful validation/inference all operators will be boolean.
 * 
 * `infer_output` ensures that all inputs are boolean type and sets the
 * output to boolean.
 * 
 * `validate` ensures that all input/output operands are boolean.
 */
class XMIPP4_CORE_API boolean_data_type_policy final
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

	const boolean_data_type_policy& get() noexcept;
};

} // namespace multidimensional
} // namespace xmipp4

