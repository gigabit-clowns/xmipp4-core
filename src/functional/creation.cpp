// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/functional/creation.hpp>

#include <xmipp4/core/ndarray/array_descriptor.hpp>
#include <xmipp4/ops/assignment/copy_operation.hpp>
#include <xmipp4/ops/assignment/fill_operation.hpp>
#include <xmipp4/core/dispatch/execute.hpp>
#include <xmipp4/core/dispatch/execution_context.hpp>
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

namespace
{

std::shared_ptr<buffer> reuse_array_storage(
	array &donor,
	std::size_t target_size,
	const memory_resource &target_resource
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

std::shared_ptr<buffer> allocate_array_storage(
	std::size_t size,
	const device_context &device_context,
	memory_allocator &allocator
)
{
	const auto &instance = device_context.get_device_instance();
	XMIPP4_ASSERT(instance);

	const auto &properties = instance->get_properties();
	const auto &queue = device_context.get_active_queue();

	const auto max_alignment = allocator.get_max_alignment();
	const auto preferred_alignment = properties.get_optimal_data_alignment();
	const auto base_alignment = std::min(max_alignment, preferred_alignment);
	const auto alignment = std::min(base_alignment, bit_ceil(size));

	return allocator.allocate(size, alignment, queue.get());
}

} // anonymous namespace


array empty(
	array_descriptor descriptor,
	memory_resource_affinity affinity,
	const execution_context &context,
	array *out
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

	std::shared_ptr<buffer> storage;
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
		return array(std::move(storage), std::move(descriptor));
	}

	if (
		out->get_storage() != storage.get() ||
		out->get_descriptor() != descriptor
	)
	{
		*out = array(std::move(storage), std::move(descriptor));
	}

	return out->share();
}

array zeros(
	array_descriptor descriptor,
	memory_resource_affinity affinity,
	const execution_context &context,
	array *out
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

array ones(
	array_descriptor descriptor,
	memory_resource_affinity affinity,
	const execution_context &context,
	array *out
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

array full(
	array_descriptor descriptor,
	memory_resource_affinity affinity,
	const scalar_value &fill_value,
	const execution_context &context,
	array *out
)
{
	std::array<array, 1> outputs = {
		empty(descriptor, affinity, context, out) 
	};

	execute(
		ops::fill_operation(fill_value),
		make_span(outputs),
		{},
		context
	);

	return std::move(outputs[0]);
}

array copy(
	const_array source,
	const execution_context &context,
	array *out
)
{
	return execute_unary(ops::copy_operation(), source, context, out);
}

void fill(
	array &out,
	const scalar_value &fill_value,
	const execution_context &context
)
{
	execute(
		ops::fill_operation(fill_value),
		make_span(&out, 1),
		{},
		context
	);
}

} // namespace xmipp4
