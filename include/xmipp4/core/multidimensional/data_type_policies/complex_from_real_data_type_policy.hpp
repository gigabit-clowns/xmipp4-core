// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/multidimensional/data_type_policy.hpp>
#include <xmipp4/core/platform/dynamic_shared_object.h>

namespace xmipp4
{
namespace multidimensional
{

/**
 * @brief Data type policy for operations that combine real valued
 * arrays into complex arrays.
 *
 * `infer_output` requires inputs to be the same real-valued type and sets the 
 * output to the corresponding complex type.
 *
 * `validate` ensures inputs are the same real-valued type and the output 
 * matches make_complex(input_type).
 */
class XMIPP4_CORE_API complex_from_real_data_type_policy final
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

	static const complex_from_real_data_type_policy& get() noexcept;

private:
	static numerical_type check_inputs(
		span<const numerical_type> input_types,
		const char *context
	);
};

} // namespace multidimensional
} // namespace xmipp4
