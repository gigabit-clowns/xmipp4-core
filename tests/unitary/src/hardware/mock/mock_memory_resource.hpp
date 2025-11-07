// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/hardware/memory_resource.hpp>

#include <trompeloeil.hpp>

namespace xmipp4
{
namespace hardware
{

class mock_memory_resource final
    : public memory_resource
{
public:
    MAKE_CONST_MOCK0(get_target_device, device*(), noexcept override);
    MAKE_CONST_MOCK0(get_kind, memory_resource_kind(), noexcept override);
    MAKE_MOCK2(
        create_memory_heap, 
        std::shared_ptr<memory_heap>(std::size_t, std::size_t), 
        override
    );

};

} // namespace hardware
} // namespace xmipp4
