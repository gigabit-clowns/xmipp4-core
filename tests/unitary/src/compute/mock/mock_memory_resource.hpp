// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/compute/memory_resource.hpp>

#include <trompeloeil.hpp>

namespace xmipp4
{
namespace compute
{

class mock_memory_resource final
    : public memory_resource
{
public:
    MAKE_MOCK0(get_target_device, device*(), const noexcept override);
    MAKE_MOCK0(get_kind, memory_resource_kind(), const noexcept override);
    MAKE_MOCK0(create_allocator, std::shared_ptr<memory_allocator>(), override);

};

} // namespace compute
} // namespace xmipp4
