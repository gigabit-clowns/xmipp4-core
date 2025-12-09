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
array* validate_output_array(array *out, hardware::memory_allocator &allocator)
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

	if (&storage->get_memory_resource() != &allocator.get_memory_resource())
	{
		XMIPP4_LOG_WARN(
			"An array was provided for reuse but it uses a different "
			"memory resource than the allocator."
		);
		return nullptr;
	}

    return out;
}

static 
std::shared_ptr<hardware::buffer> cannibalize_output_array(
	array *out, 
	std::size_t storage_requirement
)
{
    if (out)
    {
		auto out_storage = out->share_storage();
        if (out_storage && out_storage->get_size() >= storage_requirement)
        {
			return out_storage;
        }

		XMIPP4_LOG_WARN(
			"An array was provided for reuse but it does not have storage or "
			"its storage enough does not have enough capacity"
		);
    }

	return nullptr;
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
    out = validate_output_array(out, allocator);

	// Try to re-use the output array as it is
	if (out && out->get_layout() == layout && out->get_data_type() == data_type)
	{
		return out->view();
	}

	// Try to re-use the output array's storage
    const auto storage_requirement = 
		compute_storage_requirement(layout, data_type);
	auto storage = cannibalize_output_array(out, storage_requirement);

	// Allocate if necessary
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
