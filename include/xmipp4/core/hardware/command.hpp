// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/platform/dynamic_shared_object.h>
#include <xmipp4/core/numerical_type.hpp>
#include <xmipp4/core/span.hpp>

#include <memory>

namespace xmipp4 
{
namespace hardware
{

class buffer;
class command_queue;
	
/**
 * @brief Abstract representation of an executable command.
 */
class command
{
public:
	XMIPP4_CORE_API
	command() noexcept;
	command(const command &other) = delete;
	command(command &&other) = delete;
	XMIPP4_CORE_API
	virtual ~command();

	command& operator=(const command &other) = delete;
	command& operator=(command &&other) = delete;

	/**
	 * @brief Execute the command on a queue.
	 *
	 * The read_write operands refer to the first descriptors passed during
	 * construction of the command. The read_only operands refer to the last
	 * descriptors passed during the construction of the command.
	 *  
	 * @param output_operands Operands where data may be written. Neither
	 * of them may be null.
	 * @param input_operands  Read-only operands. Neither of them may be
	 * null.
	 * @param queue Optional queue where this command will be executed. Must 
	 * belong to the device used in construction.
	 */
	virtual void submit(
		span<const std::shared_ptr<hardware::buffer>> output_operands,
		span<const std::shared_ptr<const hardware::buffer>> input_operands,
		hardware::command_queue &queue
	) const = 0;
};

} // namespace hardware
} // namespace xmipp4
