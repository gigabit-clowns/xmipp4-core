// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "device_properties.hpp"

#include <utility>

namespace xmipp4
{
namespace compute
{

inline 
device_properties::device_properties()
    : m_type(device_type::unknown),
      m_name(),
      m_physical_location(),
      m_total_memory_bytes(0)
{
}

inline
void device_properties::set_type(device_type type) noexcept
{
	m_type = type;
}

inline
device_type device_properties::get_type() const noexcept
{
    return m_type;
}

template <typename Str>
inline
void device_properties::set_name(Str &&name)
{
    m_name = std::forward<Str>(name);
}

inline
const std::string& device_properties::get_name() const noexcept
{
    return m_name;
}

template <typename Str>
inline
void device_properties::set_physical_location(Str &&location)
{
    m_physical_location = std::forward<Str>(location);
}

inline
const std::string& device_properties::get_physical_location() const noexcept
{
    return m_physical_location;
}

inline
void device_properties::set_total_memory_bytes(std::size_t bytes) noexcept
{
    m_total_memory_bytes = bytes;
}

inline
std::size_t device_properties::get_total_memory_bytes() const noexcept
{
    return m_total_memory_bytes;
}

} // namespace compute
} // namespace xmipp4
