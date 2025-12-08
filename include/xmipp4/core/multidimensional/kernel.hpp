// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "../platform/dynamic_shared_object.h"
#include "../numerical_type.hpp"
#include "../span.hpp"

namespace xmipp4 
{
namespace hardware
{

class buffer;
class device_queue;
	
} // namespace hardware


namespace multidimensional
{

/**
 * @brief Abstract representation of an executable operation.
 * 
 */
class XMIPP4_CORE_API kernel
{
public:
	kernel() noexcept;
	kernel(const kernel &other) = delete;
	kernel(kernel &&other) = delete;
    virtual ~kernel();

	kernel& operator=(const kernel &other) = delete;
	kernel& operator=(kernel &&other) = delete;

	/**
	 * @brief Execute the kernel.
	 * 
	 * @param read_write_operands Operands where data may be written.
	 * @param read_only_operands  Read-only operands.
	 * @param queue Optional queue where this kernel will be executed. Must 
	 * belong to the device used in construction.
	 * 
	 */
    virtual void execute(
        span<hardware::buffer*> read_write_operands,
        span<const hardware::buffer*> read_only_operands,
		hardware::device_queue *queue
    ) const = 0;
};

} // namespace multidimensional
} // namespace xmipp4
