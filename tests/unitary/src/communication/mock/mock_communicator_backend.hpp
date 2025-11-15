// SPDX-License-Identifier: GPL-3.0-only

#pragma once

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
    MAKE_MOCK0(get_name, std::string (), const override);
    MAKE_MOCK0(get_version, version (), const override);
    MAKE_MOCK0(get_suitability, backend_priority (), const override);
    MAKE_MOCK0(create_world_communicator, std::shared_ptr<communicator> (), const override);

};

} // namespace communication
} // namespace xmipp4
