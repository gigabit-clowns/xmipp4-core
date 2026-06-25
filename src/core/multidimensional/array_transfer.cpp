// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/array_transfer.hpp>

#include <xmipp4/core/multidimensional/execution_context.hpp>
#include <xmipp4/core/multidimensional/operation_execute.hpp>
#include <xmipp4/core/multidimensional/array_creation.hpp>
#include <xmipp4/core/multidimensional/array_descriptor.hpp>
#include <xmipp4/core/multidimensional/operations/assignment/copy_operation.hpp>
#include <xmipp4/core/hardware/buffer.hpp>

namespace xmipp4 
{
namespace multidimensional
{

namespace
{

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
			strided_layout::make_contiguous_layout(make_span(input_extents)),
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

} // anonymous namespace

array to_device(array &input, const execution_context &context)
{
	const auto *storage = input.get_storage();
	if (!storage)
	{
		throw std::invalid_argument(
			"to_device requires the input to have an associated storage"
		);
	}

	//const auto &resource = storage->get_memory_resource();
	//auto &device = context.get_device();
	//if (hardware::is_device_accessible(resource, device))
	//{
	//	return input.share();
	//}

	return to_device_copy(input, context);
}

array_view to_device(array_view input, const execution_context &context)
{
	const auto *storage = input.get_storage();
	if (!storage)
	{
		throw std::invalid_argument(
			"to_device requires the input to have an associated storage"
		);
	}

	//const auto &resource = storage->get_memory_resource();
	//auto &device = context.get_device();
	//if (hardware::is_device_accessible(resource, device))
	//{
	//	return input;
	//}

	return to_device_copy(std::move(input), context);
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
	const auto *storage = input.get_storage();
	if (!storage)
	{
		throw std::invalid_argument(
			"to_host requires the input to have an associated storage"
		);
	}

	//const auto &resource = storage->get_memory_resource();
	//if (hardware::is_host_accessible(resource.get_kind()))
	//{
	//	return input.share();
	//}

	return to_host_copy(input, context);
}

array_view to_host(array_view input, const execution_context &context)
{
	const auto *storage = input.get_storage();
	if (!storage)
	{
		throw std::invalid_argument(
			"to_host requires the input to have an associated storage"
		);
	}

	//const auto &resource = storage->get_memory_resource();
	//if (hardware::is_host_accessible(resource.get_kind()))
	//{
	//	return input;
	//}

	return to_host_copy(std::move(input), context);
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
