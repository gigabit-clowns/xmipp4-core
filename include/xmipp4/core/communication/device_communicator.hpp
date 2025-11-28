// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "collective_communicator.hpp"
#include "../platform/dynamic_shared_object.h"

#include <memory>

namespace xmipp4 
{
namespace hardware
{

class device;
	
} // namespace hardware
namespace communication
{

class device_operation;
class device_send_region;
class device_receive_region;
class device_duplex_region;

/**
 * @brief Abstract class to represent inter device communications.
 * 
 */
class XMIPP4_CORE_API device_communicator
	: public collective_communicator<
		device_communicator,
		device_operation,
		device_send_region,
		device_receive_region,
		device_duplex_region
	>
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

};

} // namespace communication
} // namespace xmipp4
