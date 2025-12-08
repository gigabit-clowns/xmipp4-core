// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/operation_dispatcher.hpp>

#include <xmipp4/core/multidimensional/kernel_manager.hpp>
#include <xmipp4/core/multidimensional/kernel.hpp>
#include <xmipp4/core/multidimensional/array.hpp>
#include <xmipp4/core/multidimensional/array_factory.hpp>
#include <xmipp4/core/multidimensional/array_descriptor.hpp>
#include <xmipp4/core/multidimensional/operation.hpp>

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

		operands[i] = empty(
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
		output_operands.begin(), 
		output_operands.end(),
		output_descriptors.begin(),
		std::mem_fn(&array::get_descriptor)
	);

	std::transform(
		input_operands.begin(), 
		input_operands.end(),
		input_descriptors.begin(),
		std::mem_fn(&array::get_descriptor)
	);

	operation.validate_input(input_descriptors);
	operation.deduce_operands(output_descriptors, input_descriptors);

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
	boost::container::small_vector<storage, 16> output_storages(n_outputs);
	boost::container::small_vector<storage, 16> input_storages(n_inputs);

	std::transform(
		output_operands.begin(), 
		output_operands.end(),
		output_storages.begin(),
		[] (const auto )
	);

	kernel->execute(
		span<storage>(output_storages.data(), output_storages.size()),
		span<const storage>(input_storages.data(), input_storages.size()),
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
