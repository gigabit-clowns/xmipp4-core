// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "elementwise_operation.hpp"

#include <xmipp4/core/platform/dynamic_shared_object.h>

namespace xmipp4 
{
namespace multidimensional
{

/**
 * @brief Base class for unary bitwise operations.
 * 
 * Mandates integer data types. Input and output operands must have the same 
 * integer type.
 */
class XMIPP4_CORE_API unary_bitwise_operation
	: public unary_elementwise_operation
{
public:
	void sanitize_operands(
		span<array_descriptor> output_descriptors,
		span<array_descriptor> input_descriptors
	) const override;
};

/**
 * @brief Base class for binary bitwise operations.
 * 
 * Mandates integer data types. All input and output operands must have the same 
 * integer type.
 */
class XMIPP4_CORE_API binary_bitwise_operation
	: public binary_elementwise_operation
{
public:
	void sanitize_operands(
		span<array_descriptor> output_descriptors,
		span<array_descriptor> input_descriptors
	) const override;
};

/**
 * @brief Compute `~x` for all elements.
 */
class XMIPP4_CORE_API bitwise_negate_operation final
	: public unary_bitwise_operation
{
public:
	std::string get_name() const override;
};

/**
 * @brief Compute `x & y` for all elements.
 */
class XMIPP4_CORE_API bitwise_and_operation final
	: public binary_bitwise_operation
{
public:
	std::string get_name() const override;
};

/**
 * @brief Compute `x | y` for all elements.
 */
class XMIPP4_CORE_API bitwise_or_operation final
	: public binary_bitwise_operation
{
public:
	std::string get_name() const override;
};

/**
 * @brief Compute `x ^ y` for all elements.
 */
class XMIPP4_CORE_API bitwise_xor_operation final
	: public binary_bitwise_operation
{
public:
	std::string get_name() const override;
};

} // namespace multidimensional
} // namespace xmipp4
