// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/multidimensional/operation.hpp>
#include <xmipp4/core/platform/dynamic_shared_object.h>

namespace xmipp4 
{
namespace multidimensional
{

/**
 * @brief Base class for comparing two arrays value by value.
 *
 * `shape_policy`: `elementwise_shape_policy`
 * `data_type_policy`: `comparison_data_type_policy`
 */
class XMIPP4_CORE_API compare_operation
	: public operation
{
public:
	const shape_policy& get_shape_policy() const noexcept override;
	const data_type_policy& get_data_type_policy() const noexcept override;
};

/**
 * @brief Computes `x < y`.
 */
class XMIPP4_CORE_API less_operation final
	: public compare_operation
{
public:
	std::string get_name() const override;
	std::size_t get_output_count() const noexcept override;
	std::size_t get_input_count() const noexcept override;
};

/**
 * @brief Computes `x <= y`.
 */
class XMIPP4_CORE_API less_equal_operation final
	: public compare_operation
{
public:
	std::string get_name() const override;
	std::size_t get_output_count() const noexcept override;
	std::size_t get_input_count() const noexcept override;
};

/**
 * @brief Computes `x == y`.
 */
class XMIPP4_CORE_API equal_operation final
	: public compare_operation
{
public:
	std::string get_name() const override;
	std::size_t get_output_count() const noexcept override;
	std::size_t get_input_count() const noexcept override;
};

/**
 * @brief Computes `x != y`.
 */
class XMIPP4_CORE_API inequal_operation final
	: public compare_operation
{
public:
	std::string get_name() const override;
	std::size_t get_output_count() const noexcept override;
	std::size_t get_input_count() const noexcept override;
};

} // namespace multidimensional
} // namespace xmipp4
