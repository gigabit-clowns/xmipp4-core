// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/array_transfer.hpp>

#include <xmipp4/core/multidimensional/execution_context.hpp>
#include <xmipp4/core/multidimensional/operation_execute.hpp>
#include <xmipp4/core/multidimensional/array_creation.hpp>
#include <xmipp4/core/multidimensional/array_descriptor.hpp>
#include <xmipp4/core/multidimensional/operations/assignment/copy_operation.hpp>
#include <xmipp4/core/hardware/buffer.hpp>
#include <xmipp4/core/hardware/memory_allocator.hpp>

namespace xmipp4 
{
namespace multidimensional
{

array transfer(
	array &input, 
	hardware::memory_resource_affinity affinity,
	const execution_context &context
)
{
	const auto *storage = input.get_storage();
	if (!storage)
	{
		throw std::invalid_argument(
			"transfer: Expected an input with associated storage."
		);
	}

	const auto &allocator = 
		context.get_device_context().get_allocator(affinity);
	if (!allocator)
	{
		throw std::invalid_argument(
			"transfer: Expected a device context with an allocator for the "
			"requested affinity."
		);
	}

	const auto &source_resource = storage->get_memory_resource();
	const auto &target_resource = allocator->get_memory_resource();
	if (&source_resource == &target_resource)
	{
		return input.share();
	}

	return transfer_copy(input, affinity, context, nullptr);
}

array transfer_copy(
	array_view input, 
	hardware::memory_resource_affinity affinity,
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
			input_descriptor.get_data_type()
		),
		affinity,
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

array to_device(array &input, const execution_context &context)
{
	return transfer(
		input, 
		hardware::memory_resource_affinity::device, 
		context
	);
}

array to_device_copy(
	array_view input, 
	const execution_context &context,
	array *out
)
{
	return transfer_copy(
		std::move(input), 
		hardware::memory_resource_affinity::device, 
		context, 
		out
	);
}

array to_host(array &input, const execution_context &context)
{
	return transfer(
		input, 
		hardware::memory_resource_affinity::host, 
		context
	);
}

array to_host_copy(
	array_view input, 
	const execution_context &context,
	array *out
)
{
	return transfer_copy(
		std::move(input), 
		hardware::memory_resource_affinity::host, 
		context, 
		out
	);
}

} // namespace multidimensional
} // namespace xmipp4
