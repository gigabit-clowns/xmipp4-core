// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/eager_operation_dispatcher.hpp>

#include <xmipp4/core/multidimensional/kernel_manager.hpp>
#include <xmipp4/core/multidimensional/kernel.hpp>
#include <xmipp4/core/multidimensional/array.hpp>
#include <xmipp4/core/multidimensional/array_factory.hpp>
#include <xmipp4/core/multidimensional/array_descriptor.hpp>
#include <xmipp4/core/multidimensional/operation.hpp>
#include <xmipp4/core/hardware/device_context.hpp>
#include <xmipp4/core/hardware/memory_resource.hpp>
#include <xmipp4/core/hardware/buffer.hpp>

#include "../config.hpp"

#include <boost/container/small_vector.hpp>

namespace xmipp4 
{
namespace multidimensional
{

class eager_operation_dispatcher::implementation {};

static void allocate_output(
	span<array> operands,
	span<const array_descriptor> descriptors,
	const hardware::device_context &device_context
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
			descriptors[i], 
			hardware::target_placement::device_optimal, 
			device_context, 
			out
		);
	}
}

static bool check_storage_placement(
	const hardware::buffer& buffer, 
	hardware::device &device
)
{
	auto &memory_resource = buffer.get_memory_resource();
	return hardware::is_device_accessible(memory_resource, device);
}

static void populate_descriptors(
	span<const array> operands,
	span<array_descriptor> descriptors
)
{	std::transform(
		operands.begin(), 
		operands.end(),
		descriptors.begin(),
		std::mem_fn(&array::get_descriptor)
	);
}

static void populate_output_storages(
	span<array> operands,
	span<std::shared_ptr<hardware::buffer>> storages,
	hardware::device &device
)
{
	std::transform(
		operands.begin(), 
		operands.end(),
		storages.begin(),
		[&] (auto &arr)
		{
			auto buffer = arr.share_storage();

			XMIPP4_ASSERT( buffer );
			XMIPP4_ASSERT( check_storage_placement(*buffer, device) );

			return buffer;
		}
	);
}

static void populate_input_storages(
	span<const array> operands,
	span<std::shared_ptr<const hardware::buffer>> storages,
	hardware::device &device
)
{
	std::transform(
		operands.begin(), 
		operands.end(),
		storages.begin(),
		[&] (const auto &arr)
		{
			auto buffer = arr.share_storage();

			if (!buffer)
			{
				throw std::invalid_argument(
					"One of the input operands does not an associated storage"
				);
			}
			if (!check_storage_placement(*buffer, device))
			{
				throw std::invalid_argument(
					"One of the input operands is not accessible by the device "
					"used to execute the operation"
				);
			}

			return buffer;
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

static std::shared_ptr<kernel> prepare_kernel(
	const operation &operation,
	span<array> output_operands,
	span<const array> input_operands,
	const hardware::device_context &device_context
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
		device_context
	);

	return nullptr; // TODO
	//return manager.build_kernel(
	//	operation, 
	//	span<const array_descriptor>(descriptors.data(), descriptors.size()),
	//	device_context.get_device()
	//);
}

static void execute_kernel(
	kernel &kernel,
	span<array> output_operands,
	span<const array> input_operands,
	const hardware::device_context &device_context
)
{
	auto &device = device_context.get_device();
	auto *queue = device_context.get_active_queue().get();

	boost::container::small_vector<
		std::shared_ptr<hardware::buffer>, 
		XMIPP4_SMALL_OUTPUT_OPERAND_COUNT 
	> output_storages(output_operands.size());
	span<std::shared_ptr<hardware::buffer>> read_write_storages(
		output_storages.data(), 
		output_storages.size()
	);
	populate_output_storages(output_operands, read_write_storages, device);
	boost::container::small_vector<
		std::shared_ptr<const hardware::buffer>, 
		XMIPP4_SMALL_INPUT_OPERAND_COUNT 
	> input_storages(input_operands.size());
	span<std::shared_ptr<const hardware::buffer>> read_only_storages(
		input_storages.data(), 
		input_storages.size()
	);
	populate_input_storages(input_operands, read_only_storages, device);

	kernel.execute(read_write_storages, read_only_storages, queue);

	if (queue)
	{
		record_queues(output_operands, *queue);
		record_queues(input_operands, *queue);
	}
}



//eager_operation_dispatcher::eager_operation_dispatcher() noexcept = default;
eager_operation_dispatcher::~eager_operation_dispatcher() = default;

void eager_operation_dispatcher::dispatch(
	const operation &operation,
	span<array> output_operands,
	span<const array> input_operands,
	const hardware::device_context &device_context
)
{
	const auto kernel = prepare_kernel(
		operation,
		output_operands,
		input_operands,
		device_context
	);

	XMIPP4_ASSERT( kernel );

	execute_kernel(
		*kernel, 
		output_operands, 
		input_operands, 
		device_context
	);
}

} // namespace multidimensional
} // namespace xmipp4
