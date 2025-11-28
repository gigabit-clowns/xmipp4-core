// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/communication/device_communicator_backend.hpp>

#include <trompeloeil.hpp>

namespace xmipp4
{
namespace communication
{

class mock_device_communicator_backend final
	: public device_communicator_backend
{
public:
	MAKE_CONST_MOCK0(get_name, std::string (), override);
	MAKE_CONST_MOCK0(get_version, version (), override);
	MAKE_CONST_MOCK1(
		get_suitability, 
		backend_priority (span<hardware::device*> devices), 
		override
	);
	MAKE_CONST_MOCK3(
		create_world_communicators, 
		std::shared_ptr<device_transaction>(
			const std::shared_ptr<host_communicator> &node_communicator, 
			span<hardware::device*> devices, 
			span<std::shared_ptr<device_communicator>> out
		), 
		override
	);
};

} // namespace communication
} // namespace xmipp4
