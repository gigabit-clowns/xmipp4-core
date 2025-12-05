// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/operation_dispatcher.hpp>

#include <xmipp4/core/multidimensional/kernel_manager.hpp>
#include <xmipp4/core/multidimensional/kernel.hpp>
#include <xmipp4/core/multidimensional/array.hpp>
#include <xmipp4/core/multidimensional/operation.hpp>
#include <xmipp4/core/multidimensional/operation_schema.hpp>

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
	const auto n_outputs = output_operands.size();
	const auto n_inputs = input_operands.size();
	const auto n_operands = n_outputs + n_inputs;

	boost::container::small_vector<strided_layout, 16> layouts(n_operands);
	boost::container::small_vector<numerical_type, 16> data_types(n_operands);
	const span<strided_layout> output_layouts(
		layouts.data(), 
		n_outputs
	);
	const span<numerical_type> output_data_types(
		data_types.data(), 
		n_outputs
	);
	const span<strided_layout> input_layouts(
		layouts.data() + n_outputs, 
		n_inputs
	);
	const span<numerical_type> input_data_types(
		data_types.data() + n_outputs, 
		n_inputs
	);

	for (std::size_t i = 0; i < n_inputs; ++i)
	{
		input_layouts[i] = input_operands[i].get_layout();
		input_data_types[i] = input_operands[i].get_data_type();
	}

	const auto &schema = operation.get_operation_schema();
	schema.deduce_output(
		output_layouts,
		output_data_types,
		input_layouts,
		input_data_types
	);

	for (std::size_t i = 0; i < layouts.size(); ++i)
	{
		array *out = &output_operands[i];
		if (out->get_storage() == nullptr)
		{
			out = nullptr;
		}

		output_operands[i] = array::empty(
			layouts[i],
			data_types[i],
			allocator,
			queue,
			&output_operands[i]
		);
	}

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

	if (queue)
	{
		for (auto &storage : output_storages)
		{
			storage.record_queue(*queue);
		}
		for (auto &storage : input_storages)
		{
			storage.record_queue(*queue);
		}
	}
}

} // namespace multidimensional
} // namespace xmipp4
