// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/operation_dispatcher.hpp>

#include <xmipp4/core/multidimensional/kernel_manager.hpp>
#include <xmipp4/core/multidimensional/kernel.hpp>
#include <xmipp4/core/multidimensional/array.hpp>
#include <xmipp4/core/multidimensional/array_descriptor.hpp>
#include <xmipp4/core/multidimensional/operation.hpp>
#include <xmipp4/core/multidimensional/operation_schema.hpp>

#include <boost/container/small_vector.hpp>

namespace xmipp4 
{
namespace multidimensional
{

class operation_dispatcher::implementation {};

static void allocate_output(
	span<array> operands,
	span<const array_descriptor> descriptors,
	hardware::memory_allocator &allocator,
	hardware::device_queue *queue
)
{
	for (std::size_t i = 0; i < operands.size(); ++i)
	{
		array *out = &operands[i];
		if (out->get_storage() == nullptr)
		{
			out = nullptr;
		}

		operands[i] = array::empty(
			descriptors[i].get_layout(),
			descriptors[i].get_data_type(),
			allocator,
			queue,
			out
		);
	}
}


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

	boost::container::small_vector<array_descriptor, 16> descriptors(n_operands);
	const span<array_descriptor> output_descriptors(
		descriptors.data(), 
		n_outputs
	);
	const span<array_descriptor> input_descriptors(
		descriptors.data() + n_outputs, 
		n_inputs
	);

	std::transform(
		input_operands.begin(), 
		input_operands.end(),
		input_descriptors.begin(),
		std::mem_fn(&array::get_descriptor)
	);

	const auto &schema = operation.get_operation_schema();
	schema.deduce_output(output_descriptors, input_descriptors);
	allocate_output(
		output_operands, 
		output_descriptors, 
		allocator, 
		queue
	);

	const auto kernel = manager.build_kernel(
		operation, 
		span<const array_descriptor>(descriptors.data(), descriptors.size()),
		device
	);
	XMIPP4_ASSERT( kernel );

	/*
	boost::container::small_vector<inmutable_storage, 16> input_storages(n_inputs);

	kernel->execute(
		span<storage>(output_storages.data(), output_storages.size()),
		span<inmutable_storage>(input_storages.data(), input_storages.size()),
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
	*/
}

} // namespace multidimensional
} // namespace xmipp4
