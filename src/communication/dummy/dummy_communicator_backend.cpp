// SPDX-License-Identifier: GPL-3.0-only

/**
 * @file dummy_communicator_backend.cpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Implementation of dummy_communicator_backend.hpp
 * @date 2024-10-26
 * 
 */

#include <xmipp4/core/communication/dummy/dummy_communicator_backend.hpp>

#include <xmipp4/core/communication/dummy/dummy_communicator.hpp>
#include <xmipp4/core/communication/communicator_manager.hpp>

#include <xmipp4/core/core_version.hpp>

namespace xmipp4 
{
namespace communication
{

std::string dummy_communicator_backend::get_name() const noexcept
{
    return "dummy";
}

version dummy_communicator_backend::get_version() const noexcept
{
    return get_core_version();
}

bool dummy_communicator_backend::is_available() const noexcept
{
    return true;
}

backend_priority dummy_communicator_backend::get_priority() const noexcept
{
    return backend_priority::fallback;
}

std::shared_ptr<communicator> 
dummy_communicator_backend::create_world_communicator() const
{
    return std::make_shared<dummy_communicator>();
}

bool dummy_communicator_backend::register_at(communicator_manager &manager)
{
    return manager.register_backend(
        std::make_unique<dummy_communicator_backend>()
    );
}

} // namespace communication
} // namespace xmipp4
