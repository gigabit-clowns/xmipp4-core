// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/span.hpp>
#include <xmipp4/core/platform/dynamic_shared_object.h>

namespace xmipp4
{
namespace multidimensional
{

class strided_layout;

/**
 * @brief Interface describing the validation and deduction of array shapes.
 */
class XMIPP4_CORE_API operation_shape_policy
{
public:
	operation_shape_policy() noexcept;
	operation_shape_policy(const operation_shape_policy &other) = delete;
	operation_shape_policy(operation_shape_policy &&other) = delete;
	virtual ~operation_shape_policy();

	operation_shape_policy& operator=(const operation_shape_policy &other) = delete;
	operation_shape_policy& operator=(operation_shape_policy &&other) = delete;

	/**
	 * @brief Deduce the shape of the output operands.
	 *
	 * The shape of the output operands is deduced from the input shape. In
	 * addition, input operands may be broadcasted and validated.
	 *
	 * @param output_layouts Layouts of output operands.
	 * @param input_layouts Layouts of input operands.
	 */
	virtual void infer_output(
		span<strided_layout> output_layouts,
		span<strided_layout> input_layouts
	) const = 0;

	/**
	 * @brief Validate the shape of the operands.
	 *
	 * Output operands are considered as "given" and they are not modified.
	 * However, input operands may be broadcasted and validated to match the
	 * shape requirements.
	 *
	 * @param output_layouts Layouts of output operands.
	 * @param input_layouts Layouts of input operands.
	 */
	virtual void validate(
		span<const strided_layout> output_layouts,
		span<strided_layout> input_layouts
	) const = 0;
};

} // namespace multidimensional
} // namespace xmipp4
