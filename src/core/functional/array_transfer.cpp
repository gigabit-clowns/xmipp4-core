// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/functional/array_transfer.hpp>

#include <xmipp4/core/dispatch/execution_context.hpp>
#include <xmipp4/core/dispatch/execute.hpp>
#include <xmipp4/core/functional/array_creation.hpp>
#include <xmipp4/core/ndarray/array_descriptor.hpp>
#include <xmipp4/core/operations/assignment/copy_operation.hpp>
#include <xmipp4/core/hardware/buffer.hpp>
#include <xmipp4/core/hardware/memory_allocator.hpp>

namespace xmipp4
{
namespace ndarray
{

ndarray::array transfer(
	ndarray::array &input,
	hardware::memory_resource_affinity affinity,
	const dispatch::execution_context &context
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

ndarray::array transfer_copy(
	ndarray::array_view input,
	hardware::memory_resource_affinity affinity,
	const dispatch::execution_context &context,
	ndarray::array *out
)
{
	const auto &input_descriptor = input.get_descriptor();

	std::vector<std::size_t> input_extents;
	input_descriptor.get_layout().get_extents(input_extents);

	ndarray::array result = empty(
		ndarray::array_descriptor(
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
		operations::copy_operation(),
		make_span(&result, 1),
		make_span(&input, 1),
		context
	);
	return result;
}

ndarray::array to_device(
	ndarray::array &input,
	const dispatch::execution_context &context
)
{
	return transfer(
		input,
		hardware::memory_resource_affinity::device,
		context
	);
}

ndarray::array to_device_copy(
	ndarray::array_view input,
	const dispatch::execution_context &context,
	ndarray::array *out
)
{
	return transfer_copy(
		std::move(input), 
		hardware::memory_resource_affinity::device, 
		context, 
		out
	);
}

ndarray::array to_host(
	ndarray::array &input,
	const dispatch::execution_context &context
)
{
	return transfer(
		input,
		hardware::memory_resource_affinity::host,
		context
	);
}

ndarray::array to_host_copy(
	ndarray::array_view input,
	const dispatch::execution_context &context,
	ndarray::array *out
)
{
	return transfer_copy(
		std::move(input), 
		hardware::memory_resource_affinity::host, 
		context, 
		out
	);
}

} // namespace ndarray
} // namespace xmipp4
