// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/hardware/memory_allocator_backend.hpp>

namespace xmipp4 
{
namespace hardware
{

class memory_allocator_manager;

class caching_memory_allocator_backend final
    : public memory_allocator_backend
{
public:
    backend_priority get_suitability(
        const memory_resource &resource
    ) const noexcept override;

    std::shared_ptr<memory_allocator> create_memory_allocator(
        memory_resource &resource
    ) const override;

    static bool register_at(memory_allocator_manager &manager);

};

} // namespace hardware
} // namespace xmipp4
