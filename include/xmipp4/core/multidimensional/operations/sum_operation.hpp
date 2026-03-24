// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "../operation.hpp"
#include "../../platform/dynamic_shared_object.h"

#include <string>

namespace xmipp4 
{
namespace multidimensional
{

/**
 * @brief sum elements from two input arrays into an output array.
 */
class sum_operation
	: public operation
{
public:
	/**
	 * @brief Indices for output operands.
	 */
	enum output_operand_indices
	{
		OUTPUT_OPERAND_RESULT,

		OUTPUT_OPERAND_COUNT
	};

	/**
	 * @brief Indices for input operands.
	 */
	enum input_operand_indices
	{
		INPUT_OPERAND_X,

		INPUT_OPERAND_COUNT
	};

	/**
	 * @brief Indices for mixed operands.
	 */
	enum operand_indices
	{
		OPERAND_RESULT,
		OPERAND_X,

		OPERAND_COUNT
	};

	XMIPP4_CORE_API explicit sum_operation(std::vector<std::size_t> axes);
	XMIPP4_CORE_API ~sum_operation() override;
	
	XMIPP4_CORE_API std::string get_name() const override;
	
	XMIPP4_CORE_API std::string serialize_parameters() const override;

	XMIPP4_CORE_API
	void sanitize_operands(
		span<array_descriptor> output_descriptors,
		span<array_descriptor> input_descriptors
	) const override;

private:
	std::vector<std::size_t> m_axes;
};

} // namespace multidimensional
} // namespace xmipp4
