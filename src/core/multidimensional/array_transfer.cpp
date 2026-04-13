// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/array_transfer.hpp>

#include <xmipp4/core/execution_context.hpp>
#include <xmipp4/core/multidimensional/operation_execute.hpp>
#include <xmipp4/core/multidimensional/array_creation.hpp>
#include <xmipp4/core/multidimensional/operations/copy_operation.hpp>
#include <xmipp4/core/hardware/buffer.hpp>

namespace xmipp4 
{
namespace multidimensional
{

array to_device(array &input, const execution_context &context)
{
	const auto *storage = input.get_storage();
	if (!storage)
	{
		throw std::invalid_argument(
			"to_device requires the input to have an associated storage"
		);
	}

	const auto &resource = storage->get_memory_resource();
	auto &device = context.get_device();
	if (hardware::is_device_accessible(resource, device))
	{
		return input.share();
	}

	return to_device_copy(input, context);
}

array to_device_copy(
	array_view input, 
	const execution_context &context,
	array *out
)
{
	array result = empty(
		input.get_descriptor(),
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

array to_host(array &input, const execution_context &context)
{
	const auto *storage = input.get_storage();
	if (!storage)
	{
		throw std::invalid_argument(
			"to_host requires the input to have an associated storage"
		);
	}

	const auto &resource = storage->get_memory_resource();
	if (hardware::is_host_accessible(resource.get_kind()))
	{
		return input.share();
	}

	return to_host_copy(input, context);
}

array to_host_copy(
	array_view input, 
	const execution_context &context,
	array *out
)
{
	array result = empty(
		input.get_descriptor(),
		hardware::memory_resource_affinity::host,
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
