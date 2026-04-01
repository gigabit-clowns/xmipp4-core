// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/operation_execute.hpp>

#include <xmipp4/core/execution_context.hpp>
#include <xmipp4/core/multidimensional/array.hpp>
#include <xmipp4/core/multidimensional/array_descriptor.hpp>
#include <xmipp4/core/multidimensional/array_view.hpp>
#include <xmipp4/core/multidimensional/operation.hpp>

#include "../config.hpp"

#include <boost/container/small_vector.hpp>

#include <algorithm>

namespace xmipp4 
{
namespace multidimensional
{
namespace
{

void populate_input_descriptors(
	span<const array_view> operands,
	span<array_descriptor> descriptors
)
{	std::transform(
		operands.begin(), 
		operands.end(),
		descriptors.begin(),
		std::mem_fn(&array_view::get_descriptor)
	);
}

void populate_output_descriptors(
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

void populate_input_storages(
	span<const array_view> operands,
	span<std::shared_ptr<const hardware::buffer>> storages
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

			return buffer;
		}
	);
}

} // anonymous namespace



void execute(
	const operation &operation,
	span<array> output_operands,
	span<const array_view> input_operands,
	const execution_context &context
)
{
	const auto n_outputs = output_operands.size();
	const auto n_inputs = input_operands.size();
	const auto n_operands = n_outputs + n_inputs;

	boost::container::small_vector<
		array_descriptor, 
		XMIPP4_SMALL_OUTPUT_OPERAND_COUNT + XMIPP4_SMALL_INPUT_OPERAND_COUNT
	> descriptors(n_operands);
	boost::container::small_vector<
		std::shared_ptr<hardware::buffer>, 
		XMIPP4_SMALL_OUTPUT_OPERAND_COUNT 
	> output_storages(n_outputs);
	boost::container::small_vector<
		std::shared_ptr<const hardware::buffer>, 
		XMIPP4_SMALL_INPUT_OPERAND_COUNT 
	> input_storages(n_inputs);

	const span<array_descriptor> output_descriptors(
		descriptors.data(), 
		n_outputs
	);
	populate_output_descriptors(output_operands, output_descriptors);
	const span<array_descriptor> input_descriptors(
		descriptors.data() + n_outputs, 
		n_inputs
	);
	populate_input_descriptors(input_operands, input_descriptors);

	operation.sanitize_operands(output_descriptors, input_descriptors);

	populate_input_storages(
		input_operands, 
		span<std::shared_ptr<const hardware::buffer>>(
			input_storages.data(),
			n_inputs
		)
	);

	// TODO
	//       ___
    //      /   \
    //     |--o--|
    //     |  _  |
    //     |     |
    //     |     |
    //     |     |        C'mon,
    //     |     |\       do something...
    //     |     | \
    //     |     |  \
    //     |_____|   \
    //     |  |  |    \
    //     |  |  |     \
    //     |  |  |      \
    //     |  |  |       `--
    //     |  |  |
    //     |__|__|
}

} // namespace multidimensional
} // namespace xmipp4

