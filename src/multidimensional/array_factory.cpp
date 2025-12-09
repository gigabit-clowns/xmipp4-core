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
std::size_t compute_storage_requirement(
	const strided_layout &layout, 
	numerical_type data_type
) noexcept
{
	return layout.compute_storage_requirement() * get_size(data_type);
}

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
    strided_layout layout, 
    numerical_type data_type,
    hardware::memory_allocator &allocator,
    hardware::device_queue *queue,
    array *out
)
{
    const auto storage_requirement = 
		compute_storage_requirement(layout, data_type);
    out = validate_output_array(out, storage_requirement, allocator);

	std::shared_ptr<hardware::buffer> storage;
	if (out)
	{
		if (out->get_layout() == layout && out->get_data_type() == data_type)
		{
			return out->view(); // Re-use as it is.
		}

		storage = out->share_storage(); // Cannibalize its storage.
	}

    if (!storage)
    {
        const auto alignment = get_alignment_requirement(
            allocator, 
            storage_requirement
        );
		storage = allocator.allocate(storage_requirement, alignment, queue);
    }

    return array(
        std::move(storage),
        std::move(layout),
        data_type
    );
}

} // namespace multidimensional
} // namespace xmipp4
