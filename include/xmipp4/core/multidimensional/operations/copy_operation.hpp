// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/multidimensional/operation.hpp>
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
class XMIPP4_CORE_API copy_operation_data_type_policy
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

	static const data_type_policy& get() noexcept;
};

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

} // namespace multidimensional
} // namespace xmipp4
