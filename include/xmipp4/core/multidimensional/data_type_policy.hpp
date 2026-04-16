// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/span.hpp>
#include <xmipp4/core/numerical_type.hpp>
#include <xmipp4/core/platform/dynamic_shared_object.h>

namespace xmipp4
{
namespace multidimensional
{

/**
 * @brief Interface describing the validation and deduction of array data types.
 */
class XMIPP4_CORE_API data_type_policy
{
public:
	data_type_policy() noexcept;
	data_type_policy(const data_type_policy &other) = delete;
	data_type_policy(data_type_policy &&other) = delete;
	virtual ~data_type_policy();

	data_type_policy& operator=(const data_type_policy &other) = delete;
	data_type_policy& operator=(data_type_policy &&other) = delete;

	/**
	 * @brief Deduce the data type of the output operands.
	 *
	 * The data type of the output operands is deduced from the input shape. In
	 * addition input types may be validated.
	 *
	 * @param output_types Data types of output operands.
	 * @param input_types Data types of input operands.
	 */
	virtual void infer_output(
		span<numerical_type> output_types,
		span<const numerical_type> input_types
	) const = 0;

	/**
	 * @brief Validate the data types of the input and output operands.
	 *
	 * Validates the data types of all operands.
	 *
	 * @param output_types Data types of output operands.
	 * @param input_types Data types of input operands.
	 */
	virtual void validate(
		span<const numerical_type> output_types,
		span<const numerical_type> input_types
	) const = 0;
};

} // namespace multidimensional
} // namespace xmipp4
