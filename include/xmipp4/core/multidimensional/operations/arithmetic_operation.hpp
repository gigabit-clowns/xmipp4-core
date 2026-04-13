// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "elementwise_operation.hpp"

#include <xmipp4/core/platform/dynamic_shared_object.h>

namespace xmipp4 
{
namespace multidimensional
{

/**
 * @brief Compute `-x` for all elements.
 * 
 * Input and output must have the same data type
 */
class XMIPP4_CORE_API negate_operation
	: public unary_elementwise_operation // TODO
{
public:
	std::string get_name() const;
};

/**
 * @brief Compute `x + y` for all elements.
 */
class XMIPP4_CORE_API add_operation
	: public binary_elementwise_operation // TODO
{
public:
	std::string get_name() const;
};

/**
 * @brief Compute `x - y` for all elements.
 */
class XMIPP4_CORE_API subtract_operation
	: public binary_elementwise_operation // TODO
{
public:
	std::string get_name() const;
};

/**
 * @brief Compute `x * y` for all elements.
 */
class XMIPP4_CORE_API multiply_operation
	: public binary_elementwise_operation // TODO
{
public:
	std::string get_name() const;
};

/**
 * @brief Compute `x / y` for all elements.
 */
class XMIPP4_CORE_API divide_operation
	: public binary_elementwise_operation // TODO
{
public:
	std::string get_name() const;
};

} // namespace multidimensional
} // namespace xmipp4
