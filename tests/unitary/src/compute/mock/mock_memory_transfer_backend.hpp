// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/compute/memory_transfer_backend.hpp>

#include <trompeloeil.hpp>

namespace xmipp4
{
namespace compute
{

class mock_memory_transfer_backend final
    : public memory_transfer_backend
{
public:
    MAKE_MOCK2(
        create_transfer,
        std::shared_ptr<memory_transfer>(
            const memory_resource& src,
            const memory_resource& dst
        ),
        const,
        override
    );

};

} // namespace compute
} // namespace xmipp4
