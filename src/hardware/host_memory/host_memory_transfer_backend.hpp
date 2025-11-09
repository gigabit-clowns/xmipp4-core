// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/hardware/memory_transfer_backend.hpp>

namespace xmipp4 
{
namespace hardware
{

class memory_transfer_manager;

class host_memory_transfer_backend final
    : public memory_transfer_backend
{
public:
    host_memory_transfer_backend() = default;
    ~host_memory_transfer_backend() override = default;

    std::shared_ptr<memory_transfer> create_transfer(
        const memory_resource& src,
        const memory_resource& dst
    ) const override;

    static bool register_at(memory_transfer_manager &manager);

};

} // namespace hardware
} // namespace xmipp4
