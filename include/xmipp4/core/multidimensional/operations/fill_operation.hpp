// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "../operation.hpp"
#include "../../scalar_ref.hpp"
#include "../../platform/dynamic_shared_object.h"

#include <string>

namespace xmipp4 
{
namespace multidimensional
{

/**
 * @brief Fill an array with a constant value.
 */
class fill_operation
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
	XMIPP4_CORE_API fill_operation(scalar_ref fill_value) noexcept;
	XMIPP4_CORE_API ~fill_operation() override;
	
	XMIPP4_CORE_API std::string get_name() const override;

	XMIPP4_CORE_API
	std::string serialize_parameters() const override;

	XMIPP4_CORE_API
	void sanitize_operands(
		span<array_descriptor> output_descriptors,
		span<array_descriptor> input_descriptors
	) const override;

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
