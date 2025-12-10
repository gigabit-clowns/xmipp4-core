// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/operation_dispatcher.hpp>

#include <xmipp4/core/multidimensional/kernel_manager.hpp>
#include <xmipp4/core/multidimensional/kernel.hpp>
#include <xmipp4/core/multidimensional/array.hpp>
#include <xmipp4/core/multidimensional/array_factory.hpp>
#include <xmipp4/core/multidimensional/array_descriptor.hpp>
#include <xmipp4/core/multidimensional/operation.hpp>

#include "../config.hpp"

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

		operands[i] = empty(descriptors[i], allocator, queue, out);
	}
}

static void populate_descriptors(
	span<array> operands,
	span<array_descriptor> descriptors
)
{	std::transform(
		operands.begin(), 
		operands.end(),
		descriptors.begin(),
		std::mem_fn(&array::get_descriptor)
	);
}

static void populate_storages(
	span<array> operands,
	span<std::shared_ptr<hardware::buffer>> storages
)
{
	std::transform(
		operands.begin(), 
		operands.end(),
		storages.begin(),
		[] (auto &arr)
		{
			return arr.share_storage();
		}
	);
}

static void populate_storages(
	span<const array> operands,
	span<std::shared_ptr<const hardware::buffer>> storages
)
{
	std::transform(
		operands.begin(), 
		operands.end(),
		storages.begin(),
		[] (const auto &arr)
		{
			return arr.share_storage();
		}
	);
}

static void record_queues(
	span<const array> operands,
	hardware::device_queue &queue
)
{		
	for (const auto &operand : operands)
	{
		operand.record_queue(queue);
	}
}

std::shared_ptr<kernel> build_kernel(
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

	boost::container::small_vector<
		array_descriptor, 
		XMIPP4_SMALL_OUTPUT_OPERAND_COUNT + XMIPP4_SMALL_INPUT_OPERAND_COUNT
	> descriptors(n_operands);
	const span<array_descriptor> output_descriptors(
		descriptors.data(), 
		n_outputs
	);
	populate_descriptors(output_operands, output_descriptors);
	const span<array_descriptor> input_descriptors(
		descriptors.data() + n_outputs, 
		n_inputs
	);
	populate_descriptors(input_operands, input_descriptors);

	operation.validate_input(input_descriptors);
	operation.deduce_operands(output_descriptors, input_descriptors);

	allocate_output(
		output_operands, 
		output_descriptors, 
		allocator, 
		queue
	);

	return manager.build_kernel(
		operation, 
		span<const array_descriptor>(descriptors.data(), descriptors.size()),
		device
	);
}

static void execute_kernel(
	kernel &kernel,
	span<array> output_operands,
	span<const array> input_operands,
	hardware::device_queue *queue
)
{
	boost::container::small_vector<
		std::shared_ptr<hardware::buffer>, 
		XMIPP4_SMALL_OUTPUT_OPERAND_COUNT 
	> output_storages(output_operands.size());
	populate_storages(output_operands, output_storages);
	boost::container::small_vector<
		std::shared_ptr<const hardware::buffer>, 
		XMIPP4_SMALL_INPUT_OPERAND_COUNT 
	> input_storages(input_operands.size());
	populate_storages(input_operands, input_storages);

	kernel.execute(
		span<const std::shared_ptr<hardware::buffer>>(
			output_storages.data(), 
			output_storages.size()
		),
		span<const std::shared_ptr<const hardware::buffer>>(
			input_storages.data(), 
			input_storages.size()
		),
		queue
	);

	if (queue)
	{
		record_queues(output_operands, *queue);
		record_queues(input_operands, *queue);
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
	const auto kernel = build_kernel(
		manager,
		operation,
		output_operands,
		input_operands,
		allocator,
		device,
		queue
	);

	XMIPP4_ASSERT( kernel );
	execute_kernel(*kernel, output_operands, input_operands, queue);
}

} // namespace multidimensional
} // namespace xmipp4
