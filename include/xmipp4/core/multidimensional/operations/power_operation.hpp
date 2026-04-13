// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "elementwise_operation.hpp"

#include <xmipp4/core/platform/dynamic_shared_object.h>

namespace xmipp4 
{
namespace multidimensional
{

/**
 * @brief Compute `x^y` for all elements.
 */
class XMIPP4_CORE_API power_operation
	: public binary_elementwise_operation // TODO
{
public:
	std::string get_name() const override;
};

/**
 * @brief Compute `exp(x)` for all elements.
 */
class XMIPP4_CORE_API exp_operation
	: public unary_elementwise_operation // TODO
{
public:
	std::string get_name() const override;
};

/**
 * @brief Compute `log(x)` for all elements.
 */
class XMIPP4_CORE_API log_operation
	: public unary_elementwise_operation // TODO
{
public:
	std::string get_name() const override;
};

/**
 * @brief Compute `x^2` for all elements.
 */
class XMIPP4_CORE_API square_operation
	: public unary_elementwise_operation // TODO
{
public:
	std::string get_name() const override;
};

/**
 * @brief Compute `sqrt(x)` for all elements.
 */
class XMIPP4_CORE_API sqrt_operation
	: public unary_elementwise_operation // TODO
{
public:
	std::string get_name() const override;
};

} // namespace multidimensional
} // namespace xmipp4
