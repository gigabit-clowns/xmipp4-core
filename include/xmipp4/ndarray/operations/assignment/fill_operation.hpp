// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/dispatch/operation.hpp>
#include <xmipp4/core/scalar_value.hpp>
#include <xmipp4/core/platform/dynamic_shared_object.h>

namespace xmipp4
{

/**
 * @brief Fill an array with a constant value.
 */
class fill_operation final
	: public operation
{
public:
	/**
	 * @brief Indices for output operands.
	 */
	enum output_operand_indices
	{
		OUTPUT_OPERAND_DESTINATION,

		OUTPUT_OPERAND_COUNT
	};

	/**
	 * @brief Indices for input operands.
	 */
	enum input_operand_indices
	{
		// No input operands

		INPUT_OPERAND_COUNT
	};

	/**
	 * @brief Construct a fill operation by the value used to fill.
	 *
	 * @param fill_value Value used for filling.
	 */
	XMIPP4_CORE_API
	explicit fill_operation(const scalar_value &fill_value) noexcept;
	XMIPP4_CORE_API ~fill_operation() override;

	XMIPP4_CORE_API std::string get_name() const override;

	XMIPP4_CORE_API
	operation_arity get_arity() const noexcept override;

	XMIPP4_CORE_API
	const operation_shape_policy&
	get_operation_shape_policy() const noexcept override;

	XMIPP4_CORE_API
	const operation_data_type_policy&
	get_operation_data_type_policy() const noexcept override;

	/**
	 * @brief Get the fill value.
	 *
	 * @return const scalar_value& The fill_value
	 */
	XMIPP4_CORE_API
	const scalar_value& get_fill_value() const noexcept;

private:
	scalar_value m_fill_value;
};

} // namespace xmipp4
