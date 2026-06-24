// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/span.hpp>
#include <xmipp4/core/numerical_type.hpp>
#include <xmipp4/core/platform/dynamic_shared_object.h>

namespace xmipp4
{
namespace multidimensional
{

/**
 * @brief Strategy interface that deduces the numerical types of an operation's 
 * output arrays from its input types.
 *
 * @see operation_shape_policy
 */
class XMIPP4_CORE_API operation_data_type_policy
{
public:
	operation_data_type_policy() noexcept;
	operation_data_type_policy(const operation_data_type_policy &other) = delete;
	operation_data_type_policy(operation_data_type_policy &&other) = delete;
	virtual ~operation_data_type_policy();

	operation_data_type_policy&
	operator=(const operation_data_type_policy &other) = delete;
	operation_data_type_policy&
	operator=(operation_data_type_policy &&other) = delete;

	/**
	 * @brief Deduce the numerical types of the output tensors.
	 *
	 * Fills each element of @p output_types with the @ref numerical_type
	 * that the operation would produce for the corresponding output tensor,
	 * given the types of all input tensors in @p input_types.
	 *
	 * @param output_types Writable span whose length equals the number of
	 * output tensors; each entry is set to the deduced output type.
	 * @param input_types Read-only span whose length equals the number of
	 * input tensors; each entry holds the numerical type of the
	 * corresponding input tensor.
	 */
	virtual void deduce(
		span<numerical_type> output_types,
		span<const numerical_type> input_types
	) const = 0;
};

} // namespace multidimensional
} // namespace xmipp4
