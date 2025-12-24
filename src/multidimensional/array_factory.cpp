// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/array_factory.hpp>

#include <xmipp4/core/multidimensional/array_descriptor.hpp>
#include <xmipp4/core/multidimensional/operations/fill_operation.hpp>
#include <xmipp4/core/multidimensional/operations/copy_operation.hpp>
#include <xmipp4/core/hardware/buffer.hpp>
#include <xmipp4/core/hardware/device_queue.hpp>
#include <xmipp4/core/hardware/device_properties.hpp>
#include <xmipp4/core/hardware/memory_allocator.hpp>
#include <xmipp4/core/binary/bit.hpp>
#include <xmipp4/core/execution_context.hpp>
#include <xmipp4/core/logger.hpp>

namespace xmipp4 
{
namespace multidimensional
{

static 
std::size_t get_alignment_requirement(
    const hardware::memory_allocator &allocator,
    const hardware::device_properties &properties,
    std::size_t size
)
{
    size = binary::bit_ceil(size);
    const auto max_alignment = allocator.get_max_alignment();
    const auto preferred_alignment = properties.get_optimal_data_alignment();
    return std::min(std::min(max_alignment, preferred_alignment), size);
}

static
std::shared_ptr<hardware::buffer> reuse_array_storage(
	array *out, 
	std::size_t storage_requirement,
	const hardware::memory_allocator &allocator
)
{
	if (!out)
	{
		return nullptr;
	}

	const auto storage = out->share_storage();
	if (!storage)
	{
		XMIPP4_LOG_WARN(
			"An array was provided for reuse but it does not have any "
			"associated storage."
		);
		return nullptr;
	}

	if (storage->get_size() < storage_requirement)
	{
		XMIPP4_LOG_WARN(
			"An array was provided for reuse but its storage does not have "
			"enough capacity."
		);
		return nullptr;
	}

	if (&storage->get_memory_resource() != &allocator.get_memory_resource())
	{
		XMIPP4_LOG_WARN(
			"An array was provided for reuse but it uses a different "
			"memory resource than the provided allocator."
		);
		return nullptr;
	}

	return storage;
}

array empty(
	array_descriptor descriptor,
	hardware::memory_resource_affinity affinity,
	const execution_context &context,
	array *out
)
{
	auto &allocator = context.get_memory_allocator(affinity);
	const auto storage_requirement = compute_storage_requirement(descriptor);

	auto storage = reuse_array_storage(out, storage_requirement, allocator);
	if (storage && out->get_descriptor() == descriptor)
	{
		return out->share(); // Trivial
	}

	if (!storage)
	{
		const auto alignment = get_alignment_requirement(
			allocator, 
			context.get_device_properties(),
			storage_requirement
		);
		auto *queue = context.get_active_queue().get();
		storage = allocator.allocate(storage_requirement, alignment, queue);
	}

	array result(std::move(storage), std::move(descriptor));
	if (out)
	{
		*out = result.share();
	}

	return result;
}

array zeros(
	array_descriptor descriptor,
	hardware::memory_resource_affinity affinity,
	const execution_context &context,
	array *out
)
{
	return full(
		descriptor,
		affinity,
		0,
		context,
		out=out
	);
}

array ones(
	array_descriptor descriptor,
	hardware::memory_resource_affinity affinity,
	const execution_context &context,
	array *out
)
{
	return full(
		descriptor,
		affinity,
		1,
		context,
		out=out
	);
}

array full(
	array_descriptor descriptor,
	hardware::memory_resource_affinity affinity,
	const scalar_ref &fill_value,
	const execution_context &context,
	array *out
)
{
	auto result = empty(descriptor, affinity, context, out);

	fill_operation op(fill_value);

	// TODO dispatch

	return result;
}

array copy(
	array_view source,
	const execution_context &context,
	array *out
)
{
	auto destination = empty(
		source.get_descriptor(), 
		hardware::memory_resource_affinity::device, 
		context, 
		out
	);

	copy_operation op;

	// TODO dispatch a copy operation

	return destination;
}

} // namespace multidimensional
} // namespace xmipp4
