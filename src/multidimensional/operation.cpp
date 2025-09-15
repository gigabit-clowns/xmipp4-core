// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/operation.hpp>

#include <xmipp4/core/multidimensional/array.hpp>
#include <xmipp4/core/multidimensional/allocator.hpp>
#include <xmipp4/core/multidimensional/context.hpp>

#include <stdexcept>

namespace xmipp4 
{
namespace multidimensional
{

void operation::launch(span<array> read_write_operands, 
                       span<const array> read_only_operands, 
                       const context &context )
{
    prepare_output(read_write_operands, context);
    validate_operands(read_write_operands, read_only_operands);

    std::vector<storage *> read_write_storages;
    read_write_storages.reserve(read_write_operands.size());
    std::transform(
        read_write_operands.begin(), read_write_operands.end(),
        std::back_inserter(read_write_storages),
        [] (array &a) -> storage *
        {
            return a.get_storage();
        }
    );

    std::vector<const storage *> read_only_storages;
    read_only_storages.reserve(read_only_operands.size());
    std::transform(
        read_only_operands.begin(), read_only_operands.end(),
        std::back_inserter(read_only_storages),
        [] (const array &a) -> const storage *
        {
            return a.get_storage();
        }
    );


    XMIPP4_ASSERT( m_kernel );
    m_kernel->launch(
        m_iteration_layout, 
        make_span(read_write_storages),
        make_span(read_only_storages),
        context
    );
}

void operation::prepare_output(span<array> read_write_operands, 
                               const context &context )
{
    for (std::size_t i = 0; i < read_write_operands.size(); ++i)
    {
        if (!read_write_operands[i].get_storage())
        {
            auto *allocator = context.get_allocator();
            if (!allocator)
            {
                std::terminate(); // TODO throw
            }
            
            XMIPP4_ASSERT( i < m_layouts.size() );
            auto layout = m_layouts[i];

            const auto numerical_type = m_iteration_layout.get_data_type(i);

            const auto size = 
                layout.compute_element_count() * get_size(numerical_type);
            const auto alignment = alignof(max_align_t);

            auto storage = allocator->allocate(
                size, 
                alignment, 
                context.get_device_queue()
            );

            read_write_operands[i] = array(
                std::move(layout),
                std::move(storage),
                numerical_type
            );
        }
    }
}

} // namespace multidimensional
} // namespace xmipp4

