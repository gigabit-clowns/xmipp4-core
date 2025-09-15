// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/dispatch.hpp>

#include <xmipp4/core/multidimensional/context.hpp>
#include <xmipp4/core/multidimensional/kernel.hpp>
#include <xmipp4/core/multidimensional/storage.hpp>
#include <xmipp4/core/multidimensional/kernel_builder.hpp>
#include <xmipp4/core/multidimensional/kernel_registry.hpp>
#include <xmipp4/core/multidimensional/operation_id.hpp>

namespace xmipp4 
{
namespace compute
{

class device;
class device_backend;

} // namespace compute
namespace multidimensional
{

template <typename Storage>
static const compute::device *
get_common_device(span<Storage * const> operands, 
                  const compute::device * common_device = nullptr )
{
    if (common_device == nullptr && !operands.empty())
    {
        const auto *operand = operands.front();
        XMIPP4_ASSERT( operand );

        common_device = &(operand->get_device());
        operands = span<Storage * const>(
            std::next(operands.begin()),
            operands.end()
        );
    }

    for (const auto * operand : operands)
    {
        XMIPP4_ASSERT( operand );
        const auto &current_device = operand->get_device();
        if (common_device != &current_device)
        {
            std::terminate(); // TODO determine error and throw
        }
    }

    return common_device;
}

static const compute::device &
find_device(span<storage * const> outputs, 
            span<const storage * const> inputs,
            const context &context )
{
    const auto *device = get_common_device(outputs);
    device = get_common_device(inputs, device);

    if (!device)
    {
        device = context.get_device();
    }

    if (!device)
    {
        std::terminate(); // TODO determine error and throw
    }

    return *device;
}

static const compute::device_backend &
find_backend(const compute::device &device)
{
    // TODO
}

void dispatch(const operation_id &key,
              const kernel_access_layout &iteration_layout,
              span<storage * const> read_write_operands, 
              span<const storage * const> read_only_operands, 
              const context &context,
              const_any_reference parameters )
{
    kernel_registry registry; // TODO obtain

    const auto &device = find_device(
        read_write_operands, 
        read_only_operands, 
        context
    );
    const auto &backend = find_backend(device);
    const auto builder = registry.get_kernel_builder(key, backend);
    if (!builder)
    {
        std::terminate(); // TODO determine error and throw
    }

    const auto kernel = builder->build(
        iteration_layout, 
        context,
        parameters
    );

    if (!kernel)
    {
        std::terminate(); // TODO determine error and throw
    }

    kernel->launch(
        iteration_layout, 
        read_write_operands, 
        read_only_operands, 
        context
    );
}

} // namespace multidimensional
} // namespace xmipp4

