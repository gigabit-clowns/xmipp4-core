// SPDX-License-Identifier: GPL-3.0-only

#pragma once

/**
 * @file dummy_communicator_backend.hpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Definition of the communication::dummy_communicator_backend class
 * @date 2024-11-20
 * 
 */

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
    std::string get_name() const noexcept override;
    version get_version() const noexcept override;
    bool is_available() const noexcept override;
    backend_priority get_priority() const noexcept override;
    std::shared_ptr<communicator> create_world_communicator() const override;

    static bool register_at(communicator_manager &manager);

};

} // namespace communication
} // namespace xmipp4
