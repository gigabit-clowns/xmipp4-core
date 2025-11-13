// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/hardware/device.hpp>

#include <trompeloeil.hpp>

namespace xmipp4
{
namespace hardware
{

class mock_device final
    : public device
{
public:
    MAKE_MOCK1(
        enumerate_memory_resources, 
        void(std::vector<memory_resource*>&), 
        override
    );
    MAKE_MOCK0(
        create_device_queue, 
        std::shared_ptr<device_queue>(), 
        override
    );
    MAKE_MOCK0(
        create_device_event, 
        std::shared_ptr<device_event>(), 
        override
    );
    MAKE_MOCK0(
        create_device_to_host_event, 
        std::shared_ptr<device_to_host_event>(), 
        override
    );

};

} // namespace hardware
} // namespace xmipp4
