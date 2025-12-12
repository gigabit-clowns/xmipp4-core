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
    std::size_t size
)
{
    size = binary::bit_ceil(size);
    const std::size_t max_alignment = allocator.get_max_alignment();
    const std::size_t preferred_alignment = 256; // TODO: query device for preferred alignment
    return std::min(std::min(max_alignment, preferred_alignment), size);
}

static
array* validate_output_array(
	array *out, 
	std::size_t storage_requirement,
	hardware::memory_allocator &allocator
)
{
	if (!out)
	{
		return nullptr;
	}

	const auto* storage = out->get_storage();
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

    return out;
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
    out = validate_output_array(out, storage_requirement, allocator);

	if (out)
	{
		if (out->get_descriptor() != descriptor)
		{
			XMIPP4_LOG_DEBUG(
				"The provided output array's descriptor does not match the "
				"requested descriptor and it will be overriden."
			);

			*out = array(
				out->share_storage(),
				std::move(descriptor)
			);
		}

		return out->share();
	}

	const auto alignment = get_alignment_requirement(
		allocator, 
		storage_requirement
	);
	auto *queue = context.get_active_queue().get();
	auto storage = allocator.allocate(storage_requirement, alignment, queue);

    return array(
		std::move(storage),
        std::move(descriptor)
    );
}

XMIPP4_NODISCARD 
array empty(
    strided_layout layout, 
    numerical_type data_type,
	hardware::target_placement placement,
	const hardware::device_context &context,
    array *out
)
{
	return empty(
		array_descriptor(std::move(layout), data_type),
		placement,
		context,
		out
	);
}

XMIPP4_NODISCARD 
array empty(
	span<const std::size_t> extents, 
	numerical_type data_type,
	hardware::target_placement placement,
	const hardware::device_context &context,
	array *out
)
{
	return empty(
		strided_layout::make_contiguous_layout(extents),
		data_type,
		placement,
		context,
		out
	);
}

} // namespace multidimensional
} // namespace xmipp4
