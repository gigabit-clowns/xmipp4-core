// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "elementwise_operation.hpp"

#include <xmipp4/core/platform/dynamic_shared_object.h>

namespace xmipp4 
{
namespace multidimensional
{

/**
 * @brief Base class for bitwise operations.
 * 
 * `shape_policy`: `elementwise_shape_policy`
 * `data_type_policy`: `same_arithmetic_data_type_policy`
 */
class XMIPP4_CORE_API bitwise_operation
	: public operation
{
public:
	const shape_policy& get_shape_policy() const noexcept override;
	const data_type_policy& get_data_type_policy() const noexcept override;
};

/**
 * @brief Compute `~x` for all elements.
 */
class XMIPP4_CORE_API bitwise_negate_operation final
	: public bitwise_operation
{
public:
	std::string get_name() const override;
};

/**
 * @brief Compute `x & y` for all elements.
 */
class XMIPP4_CORE_API bitwise_and_operation final
	: public bitwise_operation
{
public:
	std::string get_name() const override;
};

/**
 * @brief Compute `x | y` for all elements.
 */
class XMIPP4_CORE_API bitwise_or_operation final
	: public bitwise_operation
{
public:
	std::string get_name() const override;
};

/**
 * @brief Compute `x ^ y` for all elements.
 */
class XMIPP4_CORE_API bitwise_xor_operation final
	: public bitwise_operation
{
public:
	std::string get_name() const override;
};

} // namespace multidimensional
} // namespace xmipp4
