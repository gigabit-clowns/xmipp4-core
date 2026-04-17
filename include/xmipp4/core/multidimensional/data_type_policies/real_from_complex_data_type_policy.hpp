// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/multidimensional/data_type_policy.hpp>
#include <xmipp4/core/platform/dynamic_shared_object.h>

namespace xmipp4
{
namespace multidimensional
{

/**
 * @brief Data type policy for operations that combine complex arrays into a 
 * real-valued array.
 *
 * `infer_output` requires inputs to be the same complex type and sets the 
 * output to the corresponding real type.
 *
 * `validate` ensures inputs are the same real-valued type and the output 
 * matches make_real(input_type).
 */
class XMIPP4_CORE_API real_from_complex_data_type_policy final
	: public data_type_policy
{
public:
	void infer_output(
		span<numerical_type> output_types,
		span<const numerical_type> input_types
	) const override;

	void validate(
		span<const numerical_type> output_types,
		span<const numerical_type> input_types
	) const override;

	static const real_from_complex_data_type_policy& get() noexcept;

private:
	static numerical_type check_inputs(
		span<const numerical_type> input_types,
		const char *context
	);
};

} // namespace multidimensional
} // namespace xmipp4
