// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/multidimensional/operation.hpp>
#include <xmipp4/core/scalar_ref.hpp>
#include <xmipp4/core/platform/dynamic_shared_object.h>

namespace xmipp4
{
namespace multidimensional
{

/**
 * @brief Copy elements from an input array to an output array.
 */
class XMIPP4_CORE_API copy_operation final
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
 * @brief Fill an array with a constant value.
 */
class fill_operation final
	: public operation
{
public:
	/**
	 * @brief Construct a fill operation by the value used to fill.
	 *
	 * @param fill_value Value used for filling.
	 * @warning The value referenced by fill_value must outlive the the
	 * fill_operation.
	 */
	XMIPP4_CORE_API explicit fill_operation(scalar_ref fill_value) noexcept;
	XMIPP4_CORE_API ~fill_operation() override;

	XMIPP4_CORE_API std::string get_name() const override;

	XMIPP4_CORE_API
	std::string serialize_parameters() const override;

	XMIPP4_CORE_API
	const shape_policy& get_shape_policy() const noexcept override;

	XMIPP4_CORE_API
	const data_type_policy& get_data_type_policy() const noexcept override;

	XMIPP4_CORE_API
	std::size_t get_output_count() const noexcept override;

	XMIPP4_CORE_API
	std::size_t get_input_count() const noexcept override;

	/**
	 * @brief Get the fill value.
	 *
	 * @return scalar_ref The fill_value
	 */
	XMIPP4_CORE_API
	scalar_ref get_fill_value() const noexcept;

private:
	scalar_ref m_fill_value;
};

} // namespace multidimensional
} // namespace xmipp4
