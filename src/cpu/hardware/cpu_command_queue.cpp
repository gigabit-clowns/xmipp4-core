// SPDX-License-Identifier: GPL-3.0-only

#include "cpu_command_queue.hpp"

#include <xmipp4/cpu/hardware/cpu_command.hpp>
#include <xmipp4/core/hardware/buffer.hpp>
#include <xmipp4/core/exceptions/invalid_operation_error.hpp>

#include <core/config.hpp>

#include <boost/container/small_vector.hpp>

namespace xmipp4
{
namespace hardware
{
namespace
{

template <std::size_t N>
boost::container::small_vector<void*, N> get_output_pointers(
	span<const std::shared_ptr<buffer>> buffers, 
	std::integral_constant<std::size_t, N> /*small_size_tag*/
)
{
	boost::container::small_vector<void*, N> result(buffers.size());
	for (std::size_t i = 0; i < buffers.size(); ++i)
	{
		auto *ptr = buffers[i]->get_host_ptr();
		if (ptr == nullptr)
		{
			throw invalid_operation_error(
				"cpu_command_queue::submit: output operand is not "
				"host-accessible (get_host_ptr returned nullptr)."
			);
		}
		result[i] = ptr;
	}

	return result;
}

template <std::size_t N>
boost::container::small_vector<const void*, N> get_input_pointers(
	span<const std::shared_ptr<const buffer>> buffers, 
	std::integral_constant<std::size_t, N> /*small_size_tag*/
)
{
	boost::container::small_vector<const void*, N> result(buffers.size());
	for (std::size_t i = 0; i < buffers.size(); ++i)
	{
		const auto *ptr = buffers[i]->get_host_ptr();
		if (ptr == nullptr)
		{
			throw invalid_operation_error(
				"cpu_command_queue::submit: input operand is not "
				"host-accessible (get_host_ptr returned nullptr)."
			);
		}
		result[i] = ptr;
	}

	return result;
}

template <std::size_t N>
boost::container::small_vector<void*, N> get_scratch_pointers(
	span<const std::shared_ptr<buffer>> buffers, 
	std::integral_constant<std::size_t, N> /*small_size_tag*/
)
{
	boost::container::small_vector<void*, N> result(buffers.size());
	for (std::size_t i = 0; i < buffers.size(); ++i)
	{
		auto *ptr = buffers[i]->get_host_ptr();
		if (ptr == nullptr)
		{
			throw invalid_operation_error(
				"cpu_command_queue::submit: scratch operand is not "
				"host-accessible (get_host_ptr returned nullptr)."
			);
		}
		result[i] = ptr;
	}

	return result;
}

} // anonymous namespace


void cpu_command_queue::submit(
	const command &command,
	span<const std::shared_ptr<buffer>> output_operands,
	span<const std::shared_ptr<const buffer>> input_operands,
	span<const std::shared_ptr<buffer>> scratch
)
{
	using small_output_size_tag = 
		std::integral_constant<std::size_t, XMIPP4_SMALL_OUTPUT_OPERAND_COUNT>;
	using small_input_size_tag = 
		std::integral_constant<std::size_t, XMIPP4_SMALL_INPUT_OPERAND_COUNT>;
	using small_scratch_size_tag = 
		std::integral_constant<std::size_t, XMIPP4_SMALL_SCRATCH_OPERAND_COUNT>;

	const auto &cpu_cmd = dynamic_cast<const cpu_command&>(command);
	const auto output_pointers = get_output_pointers(
		output_operands, 
		small_output_size_tag()
	);
	const auto input_pointers = get_input_pointers(
		input_operands, 
		small_input_size_tag()
	);
	const auto scratch_pointers = get_scratch_pointers(
		scratch,
		small_scratch_size_tag()
	);

	cpu_cmd.execute(
		make_span(output_pointers.data(), output_pointers.size()),
		make_span(input_pointers.data(), input_pointers.size()),
		make_span(scratch_pointers.data(), scratch_pointers.size())
	);
}

void cpu_command_queue::signal(event &/*event*/)
{
	// No-op, synchronous execution.
}

void cpu_command_queue::wait(const event&)
{
	// No-op, synchronous execution.
}

void cpu_command_queue::wait_until_completed() const
{
	// No-op, synchronous execution.
}

bool cpu_command_queue::is_idle() const
{
	return true;
}

} // namespace hardware
} // namespace xmipp4
