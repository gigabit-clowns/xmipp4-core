// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/array_initializers.hpp>

#include <xmipp4/core/multidimensional/array.hpp>
#include <xmipp4/core/multidimensional/strided_layout.hpp>
#include <xmipp4/core/hardware/buffer.hpp>
#include <xmipp4/core/hardware/memory_allocator.hpp>
#include <xmipp4/core/logger.hpp>

namespace xmipp4 
{
namespace multidimensional
{

static 
array* check_output_array(array *out, hardware::memory_allocator &allocator)
{
    if (out)
    {
        const auto *storage = out->get_storage();
        if (!storage)
        {
            XMIPP4_LOG_WARN(
                "An array was provided for reuse but it does not have storage."
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
    }

    return out;
}

array empty(
    const strided_layout &layout, 
    numerical_type data_type,
    hardware::memory_allocator &allocator,
    hardware::device_queue *queue,
    array *out
)
{
    out = check_output_array(out, allocator);

    // Check if we can reuse the output array as-is.
    if (out && out->get_data_type() == data_type && out->get_layout() == layout)
    {
        return out->view();
    }

    const auto storage_requirement = 
        layout.compute_storage_requirement() * get_size(data_type);
    std::shared_ptr<hardware::buffer> storage;

    // Check if we can reuse the output array's storage.
    if (out)
    {
        storage = out->share_storage();
        XMIPP4_ASSERT( storage ); // Checked in check_output_array
        if (storage->get_size() < storage_requirement)
        {
            XMIPP4_LOG_WARN(
                "An array was provided for reuse but it does not have enough "
                "storage."
            );
            storage.reset();
        }
    }
    
    // Allocate new storage if needed.
    if (!storage)
    {
        const std::size_t alignment = 64; // TODO determine
        storage = allocator.allocate(
            storage_requirement,
            alignment,
            queue
        );
    }

    XMIPP4_ASSERT( storage );
    return array(
        layout,
        std::move(storage),
        data_type
    );
}

} // namespace multidimensional
} // namespace xmipp4
