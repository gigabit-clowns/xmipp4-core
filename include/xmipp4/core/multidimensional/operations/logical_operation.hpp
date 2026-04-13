// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "elementwise_operation.hpp"

#include <xmipp4/core/platform/dynamic_shared_object.h>

namespace xmipp4 
{
namespace multidimensional
{

/**
 * @brief Base class for unary logical operations.
 * 
 * Restricts operands to boolean or integer data types. Input and output 
 * data types must be equal.
 */
class XMIPP4_CORE_API unary_logical_operation
	: public unary_elementwise_operation
{
public:
	void sanitize_operands(
		span<array_descriptor> output_descriptors,
		span<array_descriptor> input_descriptors
	) const override;
};

/**
 * @brief Base class for binary logical operations.
 * 
 * Restricts operands to boolean or integer data types. All input and 
 * output data types must be equal.
 */
class XMIPP4_CORE_API binary_logical_operation
	: public binary_elementwise_operation
{
public:
	void sanitize_operands(
		span<array_descriptor> output_descriptors,
		span<array_descriptor> input_descriptors
	) const override;
};

/**
 * @brief Compute `!x` for all elements.
 */
class XMIPP4_CORE_API logical_negate_operation final
	: public unary_logical_operation
{
public:
	std::string get_name() const override;
};

/**
 * @brief Compute `x && y` for all elements.
 */
class XMIPP4_CORE_API logical_and_operation final
	: public binary_logical_operation
{
public:
	std::string get_name() const override;
};

/**
 * @brief Compute `x || y` for all elements.
 */
class XMIPP4_CORE_API logical_or_operation final
	: public binary_logical_operation
{
public:
	std::string get_name() const override;
};

} // namespace multidimensional
} // namespace xmipp4
