// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "elementwise_operation.hpp"

#include <xmipp4/core/platform/dynamic_shared_object.h>

namespace xmipp4 
{
namespace multidimensional
{

/**
 * @brief Compute `sin(x)` for all elements.
 * 
 * Input and output dtypes must be equal and they must be floating-point types.
 */
class XMIPP4_CORE_API sin_operation final
	: public unary_elementwise_operation // TODO
{
public:
	std::string get_name() const override;
};

/**
 * @brief Compute `cos(x)` for all elements.
 * 
 * Input and output data types must be equal and they must be floating-point 
 * types.
 */
class XMIPP4_CORE_API cos_operation final
	: public unary_elementwise_operation // TODO
{
public:
	std::string get_name() const override;
};

/**
 * @brief Compute `sin(x)` and `cos(x)` for all elements.
 * 
 * Input and output data types must be equal and they must be floating-point 
 * types.
 */
class XMIPP4_CORE_API sincos_operation final
	: public elementwise_operation
{
public:
	std::string get_name() const override;

	void sanitize_operands(
		span<array_descriptor> output_descriptors,
		span<array_descriptor> input_descriptors
	) const override;
};

/**
 * @brief Compute `tan(x)` for all elements.
 * 
 * Input and output data types must be equal and they must be floating-point 
 * types.
 */
class XMIPP4_CORE_API tan_operation final
	: public unary_elementwise_operation // TODO
{
public:
	std::string get_name() const override;
};

/**
 * @brief Compute `asin(x)` for all elements.
 * 
 * Input and output data types must be equal and they must be floating-point 
 * types.
 */
class XMIPP4_CORE_API asin_operation final
	: public unary_elementwise_operation // TODO
{
public:
	std::string get_name() const override;
};

/**
 * @brief Compute `acos(x)` for all elements.
 * 
 * Input and output data types must be equal and they must be floating-point 
 * types.
 */
class XMIPP4_CORE_API acos_operation final
	: public unary_elementwise_operation // TODO
{
public:
	std::string get_name() const override;
};

/**
 * @brief Compute `atan(x)` for all elements.
 * 
 * Input and output data types must be equal and they must be floating-point 
 * types.
 */
class XMIPP4_CORE_API atan_operation final
	: public unary_elementwise_operation // TODO
{
public:
	std::string get_name() const override;
};

/**
 * @brief Compute the arc-tangent considering the quadrant.
 * 
 * Input and output data types must be equal and they must be floating-point 
 * types.
 */
class XMIPP4_CORE_API atan2_operation final
	: public binary_elementwise_operation // TODO
{
public:
	std::string get_name() const override;
};

} // namespace multidimensional
} // namespace xmipp4
