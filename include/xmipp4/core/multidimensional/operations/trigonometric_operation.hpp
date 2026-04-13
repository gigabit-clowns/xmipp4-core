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
class XMIPP4_CORE_API sin_operation
	: public unary_elementwise_operation // TODO
{
public:
	std::string get_name() const;
};

/**
 * @brief Compute `cos(x)` for all elements.
 * 
 * Input and output dtypes must be equal and they must be floating-point types.
 */
class XMIPP4_CORE_API cos_operation
	: public unary_elementwise_operation // TODO
{
public:
	std::string get_name() const;
};

/**
 * @brief Compute `sin(x)` and `cos(x)` for all elements.
 * 
 * Input and output dtypes must be equal and they must be floating-point types.
 */
class XMIPP4_CORE_API sincos_operation
	: public elementwise_operation // TODO
{
public:
	std::string get_name() const;
};

/**
 * @brief Compute `tan(x)` for all elements.
 * 
 * Input and output dtypes must be equal and they must be floating-point types.
 */
class XMIPP4_CORE_API tan_operation
	: public unary_elementwise_operation // TODO
{
public:
	std::string get_name() const;
};

/**
 * @brief Compute `asin(x)` for all elements.
 * 
 * Input and output dtypes must be equal and they must be floating-point types.
 */
class XMIPP4_CORE_API asin_operation
	: public unary_elementwise_operation // TODO
{
public:
	std::string get_name() const;
};

/**
 * @brief Compute `acos(x)` for all elements.
 * 
 * Input and output dtypes must be equal and they must be floating-point types.
 */
class XMIPP4_CORE_API acos_operation
	: public unary_elementwise_operation // TODO
{
public:
	std::string get_name() const;
};

/**
 * @brief Compute `atan(x)` for all elements.
 * 
 * Input and output dtypes must be equal and they must be floating-point types.
 */
class XMIPP4_CORE_API atan_operation
	: public unary_elementwise_operation // TODO
{
public:
	std::string get_name() const;
};

/**
 * @brief Compute the arc-tangent considering the quadrant.
 * 
 * Input and output dtypes must be equal and they must be floating-point types.
 */
class XMIPP4_CORE_API atan2_operation
	: public binary_elementwise_operation // TODO
{
public:
	std::string get_name() const;
};

} // namespace multidimensional
} // namespace xmipp4
