// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/functional/array_creation.hpp>

#include <xmipp4/core/ndarray/array_descriptor.hpp>
#include <xmipp4/core/operations/assignment/copy_operation.hpp>
#include <xmipp4/core/operations/assignment/fill_operation.hpp>
#include <xmipp4/core/execution/execute.hpp>
#include <xmipp4/core/execution/context.hpp>
#include <xmipp4/core/binary/bit.hpp>
#include <xmipp4/core/hardware/device_context.hpp>
#include <xmipp4/core/hardware/device_instance.hpp>
#include <xmipp4/core/hardware/device_properties.hpp>
#include <xmipp4/core/hardware/memory_allocator.hpp>
#include <xmipp4/core/hardware/buffer.hpp>

#include <core/logger.hpp>

#include <algorithm>
#include <memory>
#include <stdexcept>

namespace xmipp4
{
namespace ndarray
{

namespace
{

std::shared_ptr<hardware::buffer> reuse_array_storage(
	ndarray::array &donor,
	std::size_t target_size,
	const hardware::memory_resource &target_resource
)
{
	auto storage = donor.share_storage();
	if (!storage)
	{
		return nullptr;
	}

	if (&storage->get_memory_resource() != &target_resource)
	{
		XMIPP4_LOG_WARN(
			"empty: the output array storage lives on a different "
			"memory resource than the one requested by the affinity; "
			"reallocating on the requested resource."
		);
		return nullptr;
	}

	if (storage->get_size() < target_size)
	{
		return nullptr;
	}

	return storage;
}

std::shared_ptr<hardware::buffer> allocate_array_storage(
	std::size_t size,
	const hardware::device_context &device_context,
	hardware::memory_allocator &allocator
)
{
	const auto &instance = device_context.get_device_instance();
	XMIPP4_ASSERT(instance);

	const auto &properties = instance->get_properties();
	const auto &queue = device_context.get_active_queue();

	const auto max_alignment = allocator.get_max_alignment();
	const auto preferred_alignment = properties.get_optimal_data_alignment();
	const auto base_alignment = std::min(max_alignment, preferred_alignment);
	const auto alignment = std::min(base_alignment, binary::bit_ceil(size));

	return allocator.allocate(size, alignment, queue.get());
}

} // anonymous namespace



ndarray::array empty(
	ndarray::array_descriptor descriptor,
	hardware::memory_resource_affinity affinity,
	const execution::context &context,
	ndarray::array *out
)
{
	const auto &device_context = context.get_device_context();
	const auto &allocator = device_context.get_allocator(affinity);
	if (!allocator)
	{
		throw std::invalid_argument(
			"empty: the execution context has no allocator for the requested "
			"affinity; cannot allocate array storage."
		);
	}

	std::shared_ptr<hardware::buffer> storage;
	const auto size = compute_storage_requirement(descriptor);
	if (out)
	{
		storage = reuse_array_storage(
			*out, 
			size, 
			allocator->get_memory_resource()
		);
	}

	if (!storage)
	{
		storage = allocate_array_storage(size, device_context, *allocator);
	}

	if (!out)
	{
		return ndarray::array(std::move(storage), std::move(descriptor));
	}

	if (
		out->get_storage() != storage.get() ||
		out->get_descriptor() != descriptor
	)
	{
		*out = ndarray::array(std::move(storage), std::move(descriptor));
	}

	return out->share();
}

ndarray::array zeros(
	ndarray::array_descriptor descriptor,
	hardware::memory_resource_affinity affinity,
	const execution::context &context,
	ndarray::array *out
)
{
	return full(
		descriptor,
		affinity,
		0,
		context,
		out
	);
}

ndarray::array ones(
	ndarray::array_descriptor descriptor,
	hardware::memory_resource_affinity affinity,
	const execution::context &context,
	ndarray::array *out
)
{
	return full(
		descriptor,
		affinity,
		1,
		context,
		out
	);
}

ndarray::array full(
	ndarray::array_descriptor descriptor,
	hardware::memory_resource_affinity affinity,
	const scalar_value &fill_value,
	const execution::context &context,
	ndarray::array *out
)
{
	std::array<ndarray::array, 1> outputs = {
		empty(descriptor, affinity, context, out) 
	};

	execute(
		operations::fill_operation(fill_value),
		make_span(outputs),
		{},
		context
	);

	return std::move(outputs[0]);
}

ndarray::array copy(
	ndarray::array_view source,
	const execution::context &context,
	ndarray::array *out
)
{
	return execute_unary(operations::copy_operation(), source, context, out);
}

void fill(
	ndarray::array &out,
	const scalar_value &fill_value,
	const execution::context &context
)
{
	execute(
		operations::fill_operation(fill_value),
		make_span(&out, 1),
		{},
		context
	);
}

} // namespace ndarray
} // namespace xmipp4
