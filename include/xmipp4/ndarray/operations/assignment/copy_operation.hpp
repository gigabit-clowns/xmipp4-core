// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/dispatch/operation.hpp>
#include <xmipp4/core/platform/dynamic_shared_object.h>

namespace xmipp4
{

/**
 * @brief Copy elements from an input array to an output array.
 */
class XMIPP4_CORE_API copy_operation final
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
		INPUT_OPERAND_SOURCE,

		INPUT_OPERAND_COUNT
	};

	std::string get_name() const override;
	operation_arity get_arity() const noexcept override;
	const operation_shape_policy&
	get_operation_shape_policy() const noexcept override;
	const operation_data_type_policy&
	get_operation_data_type_policy() const noexcept override;
};

} // namespace xmipp4
