// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/array_cast.hpp>

#include <xmipp4/core/multidimensional/array_creation.hpp>
#include <xmipp4/core/multidimensional/array_descriptor.hpp>
#include <xmipp4/core/multidimensional/operation_execute.hpp>
#include <xmipp4/core/multidimensional/operations/assignment/copy_operation.hpp>
#include <xmipp4/core/hardware/memory_resource_affinity.hpp>

namespace xmipp4
{
namespace multidimensional
{

array cast(
	array &input, 
	numerical_type target_type,
	const execution_context &context
)
{
	const auto source_type = input.get_descriptor().get_data_type();
	if (source_type == target_type)
	{
		return input.share();
	}

	return cast_copy(
		input,
		target_type,
		context,
		nullptr
	);
}

array cast_copy(
	array_view input, 
	numerical_type target_type,
	const execution_context &context,
	array *out
)
{
	const auto &input_descriptor = input.get_descriptor();

	std::vector<std::size_t> input_extents;
	input_descriptor.get_layout().get_extents(input_extents);

	array result = empty(
		array_descriptor(
			layout::strided_layout::make_contiguous_layout(
				make_span(input_extents)
			),
			target_type
		),
		hardware::memory_resource_affinity::device,
		context,
		out
	);

	execute(
		copy_operation(),
		make_span(&result, 1),
		make_span(&input, 1),
		context
	);

	return result;
}

} // namespace multidimensional
} // namespace xmipp4
