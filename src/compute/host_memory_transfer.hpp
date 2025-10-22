// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/compute/memory_transfer.hpp>

namespace xmipp4 
{
namespace compute
{

class host_memory_transfer
    : public memory_transfer
{
public:
    void copy(
        const buffer &source, 
        buffer &destination,
        span<const copy_region> regions, 
        device_queue *queue
    ) const override;
    
    static const host_memory_transfer& get() noexcept;

private:
    static host_memory_transfer m_instance;

};

} // namespace compute
} // namespace xmipp4
