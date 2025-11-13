// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/hardware/memory_transfer_backend.hpp>

#include <trompeloeil.hpp>

namespace xmipp4
{
namespace hardware
{

class mock_memory_transfer_backend final
    : public memory_transfer_backend
{
public:
    MAKE_CONST_MOCK2(
        get_suitability,
        xmipp4::backend_priority(
            const memory_resource&, 
            const memory_resource&
        ),
        noexcept override
    );

    MAKE_MOCK2(
        create_transfer,
        std::shared_ptr<memory_transfer>(
            const memory_resource& src,
            const memory_resource& dst
        ),
        const override
    );

};

} // namespace hardware
} // namespace xmipp4
