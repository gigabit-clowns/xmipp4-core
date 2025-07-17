// SPDX-License-Identifier: GPL-3.0-only

#pragma once

/**
 * @file mock_communicator_backend.cpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Mock for communicator_backend interface.
 * @date 2024-12-10
 */

#include <xmipp4/core/communication/communicator_backend.hpp>

#include <trompeloeil.hpp>

namespace xmipp4
{
namespace communication
{

class mock_communicator_backend final
    : public communicator_backend
{
public:
    MAKE_MOCK0(get_name, std::string (), const noexcept override);
    MAKE_MOCK0(get_version, version (), const noexcept override);
    MAKE_MOCK0(is_available, bool (), const noexcept override);
    MAKE_MOCK0(get_priority, backend_priority (), const noexcept override);
    MAKE_MOCK0(create_world_communicator, std::shared_ptr<communicator> (), const override);

};

} // namespace communication
} // namespace xmipp4
