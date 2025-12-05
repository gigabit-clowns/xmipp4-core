// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/operation_dispatcher.hpp>

#include <xmipp4/core/multidimensional/kernel_manager.hpp>
#include <xmipp4/core/multidimensional/kernel.hpp>
#include <xmipp4/core/multidimensional/array.hpp>

#include <boost/container/small_vector.hpp>

namespace xmipp4 
{
namespace multidimensional
{

operation_dispatcher::operation_dispatcher() noexcept = default;
operation_dispatcher::~operation_dispatcher() = default;

void operation_dispatcher::dispatch(
	const kernel_manager &manager,
	const operation &operation,
	span<array> output_operands,
	span<const array> input_operands,
	hardware::memory_allocator &allocator,
	hardware::device &device,
	hardware::device_queue *queue
)
{

	boost::container::small_vector<strided_layout, 16> output_layouts;
	boost::container::small_vector<numerical_type, 16> output_data_types;
	// TODO deduce output layout and type.
	for (std::size_t i = 0; i < output_layouts.size(); ++i)
	{
		array *out = &output_operands[i];
		if (out->get_storage() == nullptr)
		{
			out = nullptr;
		}

		output_operands[i] = array::empty(
			std::move(output_layouts[i]),
			std::move(output_data_types[i]),
			allocator,
			queue,
			&output_operands[i]
		);
	}

	boost::container::small_vector<strided_layout, 16> layouts;
	boost::container::small_vector<numerical_type, 16> data_types;

	const auto kernel = manager.build_kernel(
		operation,
		span<const strided_layout>(layouts.data(), layouts.size()),
		span<const numerical_type>(data_types.data(), data_types.size()),
		device
	);

	boost::container::small_vector<storage, 16> output_storages;
	boost::container::small_vector<storage, 16> input_storages;

	XMIPP4_ASSERT( kernel );
	kernel->execute(
		span<storage>(output_storages.data(), output_storages.size()),
		{}, // TODO handle const storages.
		queue
	);

	// TODO record queues.
}

} // namespace multidimensional
} // namespace xmipp4
