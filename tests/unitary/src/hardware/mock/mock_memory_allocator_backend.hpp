// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/hardware/memory_allocator_backend.hpp>

#include <trompeloeil.hpp>

namespace xmipp4
{
namespace hardware
{

class mock_memory_allocator_backend final
    : public memory_allocator_backend
{
public:
    MAKE_CONST_MOCK1(
        get_suitability,
        xmipp4::backend_priority(
            const memory_resource&
        ),
        noexcept override
    );

    MAKE_CONST_MOCK1(
        create_memory_allocator,
        std::shared_ptr<memory_allocator>(
            memory_resource& src
        ),
        override
    );

};

} // namespace hardware
} // namespace xmipp4
