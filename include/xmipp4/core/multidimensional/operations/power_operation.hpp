// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "elementwise_operation.hpp"

#include <xmipp4/core/platform/dynamic_shared_object.h>

namespace xmipp4 
{
namespace multidimensional
{

/**
 * @brief Base class for logic operations.
 * 
 * `shape_policy`: `elementwise_shape_policy`
 * `data_type_policy`: `same_floating_data_type_policy`
 */
class XMIPP4_CORE_API logical_operation
	: public operation
{
public:
	const shape_policy& get_shape_policy() const noexcept override;
	const data_type_policy& get_data_type_policy() const noexcept override;
};

/**
 * @brief Compute `x^y` for all elements.
 */
class XMIPP4_CORE_API power_operation final
	: public logical_operation
{
public:
	std::string get_name() const override;
};

/**
 * @brief Compute `exp(x)` for all elements.
 */
class XMIPP4_CORE_API exp_operation final
	: public logical_operation
{
public:
	std::string get_name() const override;
};

/**
 * @brief Compute `log(x)` for all elements.
 */
class XMIPP4_CORE_API log_operation final
	: public logical_operation
{
public:
	std::string get_name() const override;
};

/**
 * @brief Compute `x^2` for all elements.
 */
class XMIPP4_CORE_API square_operation final
	: public logical_operation
{
public:
	std::string get_name() const override;
};

/**
 * @brief Compute `sqrt(x)` for all elements.
 */
class XMIPP4_CORE_API sqrt_operation final
	: public logical_operation
{
public:
	std::string get_name() const override;
};

} // namespace multidimensional
} // namespace xmipp4
