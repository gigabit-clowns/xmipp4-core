// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/communication/host_communicator_backend.hpp>

#include <memory>

namespace xmipp4 
{
namespace communication
{

class host_communicator_manager;

class dummy_host_communicator_backend final
	: public host_communicator_backend
{
public:
	std::string get_name() const override;
	version get_version() const override;
	backend_priority get_suitability() const override;
	std::shared_ptr<host_communicator> create_world_communicator() const override;

	static bool register_at(host_communicator_manager &manager);
};

} // namespace communication
} // namespace xmipp4
