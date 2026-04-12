// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/multidimensional/operation.hpp>
#include <xmipp4/core/platform/dynamic_shared_object.h>

namespace xmipp4 
{
namespace multidimensional
{

/**
 * @brief Base class for operations where output at a position only depends of 
 * the input at the same position.
 * 
 * When output is provided, it broadcasts input to the output shape. Otherwise,
 * inputs are broadcasted together and output is deduced from them.
 */
class XMIPP4_CORE_API elementwise_operation
	: public operation
{
public:
	void sanitize_operands(
		span<array_descriptor> output_descriptors,
		span<array_descriptor> input_descriptors
	) const override;

private:
	void broadcast_input_to_output(
		span<const array_descriptor> output_descriptors,
		span<array_descriptor> input_descriptors
	) const;

	void deduce_output(
		span<array_descriptor> output_descriptors,
		span<array_descriptor> input_descriptors
	) const;
};

/**
 * @brief Base class for elementwise operations with single input operand and 
 * single output operand.
 * 
 * @see elementwise_operation
 */
class XMIPP4_CORE_API unary_elementwise_operation
	: public elementwise_operation
{
	void sanitize_operands(
		span<array_descriptor> output_descriptors,
		span<array_descriptor> input_descriptors
	) const override;
};

/**
 * @brief Base class for elementwise operations with two input operands and a 
 * single output operand.
 * 
 * @see elementwise_operation
 */
class XMIPP4_CORE_API binary_elementwise_operation
	: public elementwise_operation
{
	void sanitize_operands(
		span<array_descriptor> output_descriptors,
		span<array_descriptor> input_descriptors
	) const override;
};

/**
 * @brief Base class for elementwise operation with three input operands and a 
 * single output operand.
 * 
 * @see elementwise_operation
 */
class XMIPP4_CORE_API ternary_elementwise_operation
	: public elementwise_operation
{
	ternary_elementwise_operation() noexcept;
	~ternary_elementwise_operation();

	void sanitize_operands(
		span<array_descriptor> output_descriptors,
		span<array_descriptor> input_descriptors
	) const override;
};

} // namespace multidimensional
} // namespace xmipp4
