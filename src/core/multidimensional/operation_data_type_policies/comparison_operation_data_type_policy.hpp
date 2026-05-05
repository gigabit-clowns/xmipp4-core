// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/multidimensional/operation_data_type_policy.hpp>
#include <xmipp4/core/platform/dynamic_shared_object.h>

namespace xmipp4
{
namespace multidimensional
{

/**
 * @brief Data type policy 
 * 
 * All input operands must be valid be non complex types and output must be
 * boolean.
 */
class XMIPP4_CORE_API comparison_operation_data_type_policy final
	: public operation_data_type_policy
{
public:
	void deduce(
		span<numerical_type> canonical_output_types,
		span<const numerical_type> input_types
	) const override;

	static const comparison_operation_data_type_policy& get() noexcept;

private:
	static void check_non_complex(numerical_type type);
};

} // namespace multidimensional
} // namespace xmipp4
