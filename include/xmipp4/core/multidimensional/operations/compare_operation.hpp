// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/multidimensional/operation.hpp>
#include <xmipp4/core/platform/dynamic_shared_object.h>

namespace xmipp4 
{
namespace multidimensional
{

/**
 * @brief Data type policy requiring all input operands to be non complex and
 * output boolean.
 * 
 * After successful validation/inference input operators are non-complex and
 * output operators boolean.
 * 
 * `infer_output` ensures that all inputs are non-complex type and sets the
 * output to boolean.
 * 
 * `validate` ensures that all inputs are non-complex and output is boolean.
 */
class XMIPP4_CORE_API comparison_data_type_policy final
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

	const comparison_data_type_policy& get() noexcept;
};

/**
 * @brief Base class for comparing two arrays value by value.
 *
 * `shape_policy`: `elementwise_shape_policy`
 * `data_type_policy`: `comparison_data_type_policy`
 */
class XMIPP4_CORE_API compare_operation
	: public operation
{
public:
	const shape_policy& get_shape_policy() const noexcept override;
	const data_type_policy& get_data_type_policy() const noexcept override;
};

/**
 * @brief Computes `x < y`.
 */
class XMIPP4_CORE_API less_operation final
	: public compare_operation
{
public:
	std::string get_name() const override;
};

/**
 * @brief Computes `x <= y`.
 */
class XMIPP4_CORE_API less_equal_operation final
	: public compare_operation
{
public:
	std::string get_name() const override;
};

/**
 * @brief Computes `x == y`.
 */
class XMIPP4_CORE_API equal_operation final
	: public compare_operation
{
public:
	std::string get_name() const override;
};

/**
 * @brief Computes `x != y`.
 */
class XMIPP4_CORE_API inequal_operation final
	: public compare_operation
{
public:
	std::string get_name() const override;
};

} // namespace multidimensional
} // namespace xmipp4
