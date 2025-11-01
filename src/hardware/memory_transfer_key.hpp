// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/platform/constexpr.hpp>

#include <functional>
#include <cstddef>

namespace xmipp4
{
namespace hardware
{   

class memory_resource;

class memory_transfer_key
{
public:
    XMIPP4_CONSTEXPR
    memory_transfer_key() noexcept;

    XMIPP4_CONSTEXPR
    memory_transfer_key(
        const memory_resource& source, 
        const memory_resource& destination
    ) noexcept;

    memory_transfer_key(const memory_transfer_key &other) = default;
    memory_transfer_key(memory_transfer_key &&other) noexcept = default;
    ~memory_transfer_key() = default;

    memory_transfer_key&
    operator=(const memory_transfer_key &other) = default;
    memory_transfer_key&
    operator=(memory_transfer_key &&other) noexcept = default;

    XMIPP4_CONSTEXPR
    const memory_resource* get_source() const noexcept;

    XMIPP4_CONSTEXPR
    const memory_resource* get_destination() const noexcept;

private:
    const memory_resource* m_source;
    const memory_resource* m_destination;

};

XMIPP4_CONSTEXPR
bool operator==(
    const memory_transfer_key& lhs, 
    const memory_transfer_key& rhs
) noexcept;

XMIPP4_CONSTEXPR
bool operator!=(
    const memory_transfer_key& lhs, 
    const memory_transfer_key& rhs
) noexcept;

} // namespace hardware
} // namespace xmipp4

namespace std
{

template<>
struct hash<xmipp4::hardware::memory_transfer_key>
{
    size_t operator()(
        const xmipp4::hardware::memory_transfer_key& key
    ) const noexcept;

};

} // namespace std

#include "memory_transfer_key.inl"
