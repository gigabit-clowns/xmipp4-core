// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/compute/memory_transfer.hpp>
#include <xmipp4/core/compute/buffer.hpp>
#include <xmipp4/core/compute/copy_region.hpp>

#include <trompeloeil.hpp>

namespace xmipp4
{
namespace compute
{

class mock_memory_transfer final
    : public memory_transfer
{
public:
    MAKE_MOCK4(
        copy,
        void(
            const buffer &source, 
            buffer &destination,
            span<const copy_region> regions, 
            device_queue *queue
        ),
        const override
    );

};

} // namespace compute
} // namespace xmipp4
