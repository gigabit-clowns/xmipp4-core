// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "communicator.hpp"
#include "../platform/dynamic_shared_object.h"

#include <memory>
#include <cstddef>

namespace xmipp4 
{
namespace hardware
{

class device;
class device_queue;

}

namespace communication
{

/**
 * @brief Abstract class to represent inter device communications.
 * 
 */
class XMIPP4_CORE_API device_communicator
	: public communicator
{
public:
	device_communicator() = default;
	device_communicator(const device_communicator &other) = delete;
	device_communicator(device_communicator &&other) = delete;
	~device_communicator() override = default;

	device_communicator& operator=(const device_communicator &other) = delete;
	device_communicator& operator=(device_communicator &&other) = delete;

	/**
	 * @brief Get the device associated to this communicator.
	 * 
	 * @return hardware::device& The device associated to this communicator.
	 */
	virtual hardware::device& get_device() const noexcept = 0;

	/**
	 * @brief Split the current communicator.
	 * 
	 * Split a communicator into multiple communicators containing.
	 * 
	 * @param colour The group where the current rank will be assigned to.
	 * @param rank_priority Hint to assign the rank in the new communicator.
	 * @return std::shared_ptr<device_communicator> The communicator where
	 * the current rank has been assigned to.
	 * 
	 */
	virtual std::shared_ptr<device_communicator> split(
		int colour, 
		int rank_priority
	) const = 0;

	/**
	 * @brief Synchronize all peers.
	 * 
	 * Wait until all peers have reached this call.
	 * 
	 * @note All ranks of the communicator need to call this method.
	 * 
	 */
	virtual void barrier(hardware::device_queue *queue) = 0;

};

} // namespace communication
} // namespace xmipp4
