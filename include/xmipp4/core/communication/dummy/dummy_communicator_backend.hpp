// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "../communicator_backend.hpp"

#include <memory>

namespace xmipp4 
{
namespace communication
{

class communicator_manager;

class dummy_communicator_backend final
	: public communicator_backend
{
public:
	std::string get_name() const override;
	version get_version() const override;
	backend_priority get_suitability() const override;
	std::shared_ptr<communicator> create_world_communicator() const override;

	static bool register_at(communicator_manager &manager);
};

} // namespace communication
} // namespace xmipp4
