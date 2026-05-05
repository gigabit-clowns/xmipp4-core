// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/multidimensional/operation_data_type_policy.hpp>
#include <xmipp4/core/platform/dynamic_shared_object.h>

namespace xmipp4
{
namespace multidimensional
{

/**
 * @brief Data type policy for operations that combine complex arrays into a 
 * real-valued array.
 * 
 * Inputs must be the same complex type and the output must match the
 * result of `make_real(input_type)`.
 */
class XMIPP4_CORE_API real_from_complex_operation_data_type_policy final
	: public operation_data_type_policy
{
public:
	void deduce_output(
		span<numerical_type> output_types,
		span<const numerical_type> input_types
	) const override;

	void validate(
		span<const numerical_type> output_types,
		span<const numerical_type> input_types
	) const override;

	static const real_from_complex_operation_data_type_policy& get() noexcept;

private:
	static numerical_type check_inputs(
		span<const numerical_type> input_types,
		const char *context
	);
};

} // namespace multidimensional
} // namespace xmipp4
