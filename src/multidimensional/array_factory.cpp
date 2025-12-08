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
array* check_output_array(array *out, hardware::memory_allocator &allocator)
{
	const storage* storage;
    if (out && (storage = out->get_storage()))
    {
        if (storage->get_memory_resource() != &allocator.get_memory_resource())
        {
            XMIPP4_LOG_WARN(
                "An array was provided for reuse but it uses a different "
                "memory resource than the allocator."
            );
            return nullptr;
        }
    }

    return out;
}

static 
storage cannibalize_output_array(
	array *out, 
	std::size_t storage_requirement
)
{
    if (out)
    {
		auto *out_storage = out->get_storage();
        if (out_storage && out_storage->get_size() >= storage_requirement)
        {
			return out_storage->share();
        }

		XMIPP4_LOG_WARN(
			"An array was provided for reuse but it does not have enough "
			"storage."
		);
    }

	return storage();
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

    out = check_output_array(out, allocator);
	auto storage = cannibalize_output_array(out, storage_requirement);
    if (storage.get_buffer() != nullptr)
    {
        const auto alignment = get_alignment_requirement(
            allocator, 
            storage_requirement
        );
        storage.rebind(
			allocator.allocate(
				storage_requirement,
				alignment,
				queue
  			)
		);
    }

    return array(
        std::move(layout),
        std::move(storage),
        data_type
    );
}

} // namespace multidimensional
} // namespace xmipp4
