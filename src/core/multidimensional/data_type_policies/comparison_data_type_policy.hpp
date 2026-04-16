// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/multidimensional/data_type_policy.hpp>
#include <xmipp4/core/platform/dynamic_shared_object.h>

namespace xmipp4
{
namespace multidimensional
{

/**
 * @brief Data type policy requiring all input operands to be non complex and
 * output boolean.
 *
 * After successful validation/inference input operators are non-complex and
 * output operators boolean.
 *
 * `infer_output` ensures that all inputs are non-complex type and sets the
 * output to boolean.
 *
 * `validate` ensures that all inputs are non-complex and output is boolean.
 */
class XMIPP4_CORE_API comparison_data_type_policy final
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

	static const comparison_data_type_policy& get() noexcept;

private:
	static void check_non_complex(numerical_type type);
};

} // namespace multidimensional
} // namespace xmipp4
