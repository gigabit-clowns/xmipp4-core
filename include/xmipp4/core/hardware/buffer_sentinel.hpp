// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "../platform/dynamic_shared_object.h"

#include <memory>

namespace xmipp4 
{
namespace hardware
{

class device_queue;

/**
 * @brief Abstract class to inform a memory allocator about the status of
 * an allocation.
 * 
 */
class XMIPP4_CORE_API buffer_sentinel
{
public:
	buffer_sentinel() = default;
	buffer_sentinel(const buffer_sentinel &other) = delete;
	buffer_sentinel(buffer_sentinel &&other) = delete;
	virtual ~buffer_sentinel() = default;

	buffer_sentinel& 
	operator=(const buffer_sentinel &other) = delete;
	buffer_sentinel& 
	operator=(buffer_sentinel &&other) = delete;

	/**
	 * @brief Signal that the allocation is being used on extra queues.
	 * 
	 * @param queue Queue where the allocation is being used.
	 * @param exclusive Whether the new queue is explicitly synchronized
	 * with the previous usage-s.
	 */
	virtual void record_queue(device_queue &queue, bool exclusive) = 0;

}; 

} // namespace hardware
} // namespace xmipp4
