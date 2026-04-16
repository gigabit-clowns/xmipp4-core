// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/multidimensional/operation.hpp>
#include <xmipp4/core/platform/dynamic_shared_object.h>

namespace xmipp4 
{
namespace multidimensional
{

/**
 * @brief Base class for arithmetic operations.
 * 
 * `shape_policy`: `elementwise_shape_policy`
 * `data_type_policy`: `same_arithmetic_data_type_policy`
 */
class XMIPP4_CORE_API arithmetic_operation
	: public operation
{
public:
	const shape_policy& get_shape_policy() const noexcept override;
	const data_type_policy& get_data_type_policy() const noexcept override;
};

/**
 * @brief Compute `-x` for all elements.
 */
class XMIPP4_CORE_API negate_operation final
	: public arithmetic_operation
{
public:
	std::string get_name() const override;
	std::size_t get_output_count() const noexcept override;
	std::size_t get_input_count() const noexcept override;
};

/**
 * @brief Compute `conj(x)` for all elements.
 */
class XMIPP4_CORE_API conjugate_operation final
	: public operation
{
public:
	std::string get_name() const override;
	const shape_policy& get_shape_policy() const noexcept override;
	const data_type_policy& get_data_type_policy() const noexcept override;
	std::size_t get_output_count() const noexcept override;
	std::size_t get_input_count() const noexcept override;
};

/**
 * @brief Compute `abs(x)` for all elements.
 */
class XMIPP4_CORE_API abs_operation final
	: public arithmetic_operation
{
public:
	std::string get_name() const override;
	std::size_t get_output_count() const noexcept override;
	std::size_t get_input_count() const noexcept override;
};


/**
 * @brief Compute `x + y` for all elements.
 */
class XMIPP4_CORE_API add_operation final
	: public arithmetic_operation
{
public:
	std::string get_name() const override;
	std::size_t get_output_count() const noexcept override;
	std::size_t get_input_count() const noexcept override;
};

/**
 * @brief Compute `x - y` for all elements.
 */
class XMIPP4_CORE_API subtract_operation final
	: public arithmetic_operation
{
public:
	std::string get_name() const override;
	std::size_t get_output_count() const noexcept override;
	std::size_t get_input_count() const noexcept override;
};

/**
 * @brief Compute `x * y` for all elements.
 */
class XMIPP4_CORE_API multiply_operation final
	: public arithmetic_operation
{
public:
	std::string get_name() const override;
	std::size_t get_output_count() const noexcept override;
	std::size_t get_input_count() const noexcept override;
};

/**
 * @brief Compute `x / y` for all elements.
 */
class XMIPP4_CORE_API divide_operation final
	: public arithmetic_operation
{
public:
	std::string get_name() const override;
	std::size_t get_output_count() const noexcept override;
	std::size_t get_input_count() const noexcept override;
};

} // namespace multidimensional
} // namespace xmipp4
