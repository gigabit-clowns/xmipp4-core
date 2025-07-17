// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "../device_buffer.hpp"
#include "../host_buffer.hpp"

namespace xmipp4 
{
namespace compute
{

/**
 * @brief Interface merging the interfaces of host_buffer and device_buffer.
 * 
 * This interface unifies host_buffer and device_buffer interfaces, as
 * when using the host as a compute device, both buffers types are 
 * equivalent
 * 
 */
class cpu_unified_buffer
    : public device_buffer
    , public host_buffer
{
public:
    std::size_t get_size() const noexcept override = 0;
    void* get_data() noexcept override = 0;
    const void* get_data() const noexcept override = 0;

    cpu_unified_buffer* get_device_accessible_alias() noexcept final;
    const cpu_unified_buffer* get_device_accessible_alias() const noexcept final;
    cpu_unified_buffer* get_host_accessible_alias() noexcept final;
    const cpu_unified_buffer* get_host_accessible_alias() const noexcept final;
    
    void record_queue(device_queue &queue) override = 0;

}; 

} // namespace compute
} // namespace xmipp4
