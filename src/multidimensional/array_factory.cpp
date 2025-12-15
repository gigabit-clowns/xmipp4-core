// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/array_factory.hpp>

#include <xmipp4/core/multidimensional/array_descriptor.hpp>
#include <xmipp4/core/hardware/buffer.hpp>
#include <xmipp4/core/hardware/device_queue.hpp>
#include <xmipp4/core/hardware/memory_allocator.hpp>
#include <xmipp4/core/binary/bit.hpp>
#include <xmipp4/core/logger.hpp>

namespace xmipp4 
{
namespace multidimensional
{

static 
std::size_t get_alignment_requirement(
    const hardware::memory_allocator &allocator,
    const hardware::device_context &context,
    std::size_t size
)
{
    size = binary::bit_ceil(size);
    const auto max_alignment = allocator.get_max_alignment();
    const auto preferred_alignment = context.get_optimal_data_alignment();
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

XMIPP4_NODISCARD 
array empty(
	array_descriptor descriptor,
	hardware::target_placement placement,
	const hardware::device_context &context,
	array *out
)
{
	auto &allocator = context.get_memory_allocator(placement);
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
			context,
			storage_requirement
		);
		auto *queue = context.get_active_queue().get();
		storage = allocator.allocate(storage_requirement, alignment, queue);
	}

	return array(std::move(storage), std::move(descriptor));
}

} // namespace multidimensional
} // namespace xmipp4
