// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/communication/host_communicator_backend.hpp>

#include <trompeloeil.hpp>

namespace xmipp4
{
namespace communication
{

class mock_host_communicator_backend final
	: public host_communicator_backend
{
public:
	MAKE_CONST_MOCK0(get_name, std::string (), override);
	MAKE_CONST_MOCK0(get_version, version (), override);
	MAKE_CONST_MOCK0(get_suitability, backend_priority (), override);
	MAKE_CONST_MOCK0(
		create_world_communicator, 
		std::shared_ptr<host_communicator>(), 
		override
	);
};

} // namespace communication
} // namespace xmipp4
