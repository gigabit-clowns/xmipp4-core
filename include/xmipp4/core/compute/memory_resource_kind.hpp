// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "../platform/dynamic_shared_object.h"

namespace xmipp4 
{
namespace compute
{

/**
 * @brief Describes the nature of a memory resource.
 * 
 */
enum class memory_resource_kind
{
    /// Memory resource lives locally on the device and can not be accessed 
    /// from the host.
    device_local,

    /// Memory resource resides in the host although it can be accessed from
    /// the device at a certain performance penalty.
    device_mapped,

    /// Memory resource resides in the host and can not be accessed from the
    /// device. However, contents can be transferred from and to the device.
    host_staging,

    /// The device and host share the same physical memory, such that both can 
    /// access it efficiently.
    unified,

    /// The device and host have their own physical memory, although transfers
    /// are managed by the driver. Therefore, buffers can be treated as if they
    /// were residing in unified memory.
    managed,

};



/**
 * @brief Check if the memory kind can be directly accessed from the host.
 * 
 * @param kind Memory resource kind to be checked.
 * @return true If the memory resource can be accessed from the host.
 * @return false If the memory resource can not be accessed from the host.
 * or an invalid kind was given.
 */
XMIPP4_CORE_API
bool is_host_accessible(memory_resource_kind kind) noexcept;

/**
 * @brief Check if the memory kind can be directly accessed from the device.
 * 
 * @param kind Memory resource kind to be checked.
 * @return true If the memory resource can be accessed from the device.
 * @return false If the memory resource can not be accessed from the device.
 * or an invalid kind was given.
 */
XMIPP4_CORE_API
bool is_device_accessible(memory_resource_kind kind) noexcept;

} // namespace compute
} // namespace xmipp4
