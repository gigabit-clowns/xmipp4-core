// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/hardware/memory_resource.hpp>

namespace xmipp4 
{
namespace hardware
{

class host_memory_resource final
    : public memory_resource
{
public:
    ~host_memory_resource() override = default;

    device* get_target_device() const noexcept override;

    memory_resource_kind get_kind() const noexcept override;

    std::shared_ptr<memory_heap> create_memory_heap(
        std::size_t size,
        std::size_t alignment
    ) override;

    static host_memory_resource& get() noexcept;

private:
    host_memory_resource() = default; // Singleton
    host_memory_resource(const host_memory_resource &other) = delete; // Singleton
    host_memory_resource(host_memory_resource &&other) = delete; // Singleton

    static host_memory_resource m_instance;

};

} // namespace hardware
} // namespace xmipp4
