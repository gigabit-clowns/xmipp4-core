// SPDX-License-Identifier: GPL-3.0-only

#include "dummy_host_communicator_backend.hpp"

#include "dummy_host_communicator.hpp"

#include <xmipp4/core/communication/host_communicator_manager.hpp>

#include <xmipp4/core/core_version.hpp>

namespace xmipp4 
{
namespace communication
{

std::string dummy_host_communicator_backend::get_name() const
{
	return "dummy";
}

version dummy_host_communicator_backend::get_version() const
{
	return get_core_version();
}

backend_priority dummy_host_communicator_backend::get_suitability() const
{
	return backend_priority::fallback;
}

std::shared_ptr<host_communicator> 
dummy_host_communicator_backend::create_world_communicator() const
{
	return std::make_shared<dummy_host_communicator>();
}

bool dummy_host_communicator_backend::register_at(
	host_communicator_manager &manager
)
{
	return manager.register_backend(
		std::make_unique<dummy_host_communicator_backend>()
	);
}

} // namespace communication
} // namespace xmipp4
