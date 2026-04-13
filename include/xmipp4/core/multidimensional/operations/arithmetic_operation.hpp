// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "elementwise_operation.hpp"

#include <xmipp4/core/platform/dynamic_shared_object.h>

namespace xmipp4 
{
namespace multidimensional
{

/**
 * @brief Base class for unary arithmetic operations.
 * 
 * Mandates arithmetic data types. Input and output operands must have the same 
 * data type.
 */
class XMIPP4_CORE_API unary_arithmetic_operation
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
 * Mandates arithmetic data types. All input and output operands must have the 
 * same data type.
 */
class XMIPP4_CORE_API binary_arithmetic_operation
	: public binary_elementwise_operation
{
public:
	void sanitize_operands(
		span<array_descriptor> output_descriptors,
		span<array_descriptor> input_descriptors
	) const override;
};

/**
 * @brief Compute `-x` for all elements.
 */
class XMIPP4_CORE_API negate_operation final
	: public unary_arithmetic_operation
{
public:
	std::string get_name() const override;
};

/**
 * @brief Compute `x + y` for all elements.
 */
class XMIPP4_CORE_API add_operation final
	: public binary_arithmetic_operation
{
public:
	std::string get_name() const override;
};

/**
 * @brief Compute `x - y` for all elements.
 */
class XMIPP4_CORE_API subtract_operation final
	: public binary_arithmetic_operation
{
public:
	std::string get_name() const override;
};

/**
 * @brief Compute `x * y` for all elements.
 */
class XMIPP4_CORE_API multiply_operation final
	: public binary_arithmetic_operation
{
public:
	std::string get_name() const override;
};

/**
 * @brief Compute `x / y` for all elements.
 */
class XMIPP4_CORE_API divide_operation final
	: public binary_arithmetic_operation
{
public:
	std::string get_name() const override;
};

} // namespace multidimensional
} // namespace xmipp4
