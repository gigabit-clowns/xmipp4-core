// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/multidimensional/operation_data_type_policy.hpp>
#include <xmipp4/core/platform/dynamic_shared_object.h>

namespace xmipp4
{
namespace multidimensional
{

/**
 * @brief Data type policy allowing converting an input type into an
 * output type.
 *
 * `infer_output` sets the output to the input type.
 *
 * `validate` ensures that all input can be converted into the output type.
 */
class XMIPP4_CORE_API copy_operation_data_type_policy final
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

	static const copy_operation_data_type_policy& get() noexcept;
};

} // namespace multidimensional
} // namespace xmipp4
