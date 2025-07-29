// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "device_create_parameters.hpp"

#include <utility>

namespace xmipp4
{
namespace compute
{

XMIPP4_INLINE_CONSTEXPR 
device_create_parameters::device_create_parameters() noexcept
    : m_desired_queue_count(1)
{
}

XMIPP4_INLINE_CONSTEXPR 
void device_create_parameters::set_desired_queue_count(std::size_t count) noexcept
{
    m_desired_queue_count = count;
}

XMIPP4_INLINE_CONSTEXPR
std::size_t device_create_parameters::get_desired_queue_count() const noexcept
{
    return m_desired_queue_count;
}


} // namespace compute
} // namespace xmipp4
