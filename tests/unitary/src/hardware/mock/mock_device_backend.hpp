// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/hardware/device_backend.hpp>

#include <trompeloeil.hpp>

namespace xmipp4
{
namespace hardware
{

class mock_device_backend final
    : public device_backend
{
public:
    MAKE_MOCK0(get_name, std::string (), const noexcept override);
    MAKE_MOCK0(get_version, version (), const noexcept override);
    MAKE_MOCK0(is_available, bool (), const noexcept override);
    MAKE_MOCK0(get_priority, backend_priority (), const noexcept override);
    MAKE_MOCK1(enumerate_devices, void (std::vector<std::size_t>&), const override);
    MAKE_MOCK2(get_device_properties, bool (std::size_t, device_properties&), const override);
    MAKE_MOCK1(create_device, std::shared_ptr<device> (std::size_t), override);

};

} // namespace hardware
} // namespace xmipp4
