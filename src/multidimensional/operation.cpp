// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/operation.hpp>

#include <xmipp4/core/multidimensional/array.hpp>
#include <xmipp4/core/multidimensional/kernel_registry.hpp>
#include <xmipp4/core/multidimensional/kernel_builder.hpp>
#include <xmipp4/core/multidimensional/kernel_access_layout_builder.hpp>
#include <xmipp4/core/multidimensional/kernel_access_layout.hpp>
#include <xmipp4/core/multidimensional/allocator.hpp>
#include <xmipp4/core/multidimensional/operation_schema.hpp>
#include <xmipp4/core/multidimensional/context.hpp>

#include <stdexcept>

namespace xmipp4 
{
namespace multidimensional
{

operation::operation(
	kernel_access_layout m_access_layout,
	std::shared_ptr<kernel> m_kernel
)
	: m_access_layout(std::move(m_access_layout))
	, m_kernel(std::move(m_kernel))
{
}

void operation::launch(
	span<array> read_write_operands, 
	span<const array> read_only_operands, 
	const context &context
)
{
	// allocate_output(read_write_operands, context);
	// validate_operands(read_write_operands, read_only_operands);

	// TODO extract as function
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
		m_access_layout, 
		make_span(read_write_storages),
		make_span(read_only_storages),
		context
	);
}

/*
void operation::allocate_output(
	span<array> read_write_operands, 
	const context &context
)
{
	auto *allocator = context.get_allocator();
	if (!allocator)
	{
		std::terminate(); // TODO throw
	}

	for (std::size_t i = 0; i < read_write_operands.size(); ++i)
	{
		if (!read_write_operands[i].get_storage())
		{
			XMIPP4_ASSERT( i < m_layouts.size() );
			auto layout = m_layouts[i];

			const auto numerical_type = m_access_layout.get_data_type(i);
			const auto element_size = get_size(numerical_type);
			const auto element_count = layout.compute_element_count();
			const auto size = element_size * element_count;
			const auto alignment = alignof(max_align_t); // TODO determine

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
*/

operation make_operation(
    const operation_id &id,
    const compute::device_backend &backend,
    span<const strided_layout> layouts,
    span<const numerical_type> data_types,
    const context &context
)
{
	if (layouts.size() != data_types.size())
	{
		throw std::invalid_argument("Layouts and data types size mismatch");
	}

	const kernel_registry registry; // TODO obtain from context
	const auto *kernel_builder = registry.get_kernel_builder(id, backend);
	if (!kernel_builder)
	{
		std::terminate(); // TODO throw
	}

	kernel_access_layout access_layout; // TODO build from schema
	auto kernel = kernel_builder->build(access_layout, context);

	return operation(
		std::move(access_layout),
		std::move(kernel)
	);
}	

} // namespace multidimensional
} // namespace xmipp4

