// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "../device_event.hpp"
#include "../device_to_host_event.hpp"

namespace xmipp4 
{
namespace hardware
{

class cpu_event final
    : public device_event
    , public device_to_host_event
{
public:
    void signal(device_queue &queue) override;

    void wait() const override;
    void wait(device_queue &queue) const override;

    bool is_signaled() const override;

}; 

} // namespace hardware
} // namespace xmipp4
