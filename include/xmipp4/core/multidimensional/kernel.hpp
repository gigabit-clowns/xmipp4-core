// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "../platform/dynamic_shared_object.h"
#include "../numerical_type.hpp"
#include "../span.hpp"

#include <memory>

namespace xmipp4 
{
namespace hardware
{

class buffer;
class device_executor;
	
} // namespace hardware

namespace multidimensional
{

/**
 * @brief Abstract representation of an executable operation core.
 */
class kernel
{
public:
	XMIPP4_CORE_API
	kernel() noexcept;
	kernel(const kernel &other) = delete;
	kernel(kernel &&other) = delete;
	XMIPP4_CORE_API
	virtual ~kernel();

	kernel& operator=(const kernel &other) = delete;
	kernel& operator=(kernel &&other) = delete;

	/**
	 * @brief Execute the kernel.
	 *
	 * The read_write operands refer to the first descriptors passed during
	 * construction of the kernel. The read_only operands refer to the last
	 * descriptors passed during the construction of the kernel.
	 *  
	 * @param output_operands Operands where data may be written. Neither
	 * of them may be null.
	 * @param input_operands  Read-only operands. Neither of them may be
	 * null.
	 * @param queue Optional queue where this kernel will be executed. Must 
	 * belong to the device used in construction.
	 */
	virtual void execute(
		span<const std::shared_ptr<hardware::buffer>> output_operands,
		span<const std::shared_ptr<const hardware::buffer>> input_operands,
		hardware::device_executor *queue
	) const = 0;
};

} // namespace multidimensional
} // namespace xmipp4
