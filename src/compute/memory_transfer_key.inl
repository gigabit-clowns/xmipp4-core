// SPDX-License-Identifier: GPL-3.0-only

#include "memory_transfer_key.hpp"

namespace xmipp4
{
namespace compute
{   

XMIPP4_INLINE_CONSTEXPR
memory_transfer_key::memory_transfer_key() noexcept
    : m_source(nullptr)
    , m_destination(nullptr)
{
}

XMIPP4_INLINE_CONSTEXPR
memory_transfer_key::memory_transfer_key(
    const memory_resource& source, 
    const memory_resource& destination
) noexcept
    : m_source(&source)
    , m_destination(&destination)
{
}

XMIPP4_INLINE_CONSTEXPR
const memory_resource* memory_transfer_key::get_source() const noexcept
{
    return m_source;
}

XMIPP4_INLINE_CONSTEXPR
const memory_resource* memory_transfer_key::get_destination() const noexcept
{
    return m_destination;  
}

XMIPP4_INLINE_CONSTEXPR
bool operator==(
    const memory_transfer_key& lhs, 
    const memory_transfer_key& rhs
) noexcept
{
    return (lhs.get_source() == rhs.get_source()) &&
           (lhs.get_destination() == rhs.get_destination());
}

XMIPP4_INLINE_CONSTEXPR
bool operator!=(
    const memory_transfer_key& lhs, 
    const memory_transfer_key& rhs
) noexcept
{
    return !(lhs == rhs);
}

} // namespace compute
} // namespace xmipp4

namespace std
{

inline
size_t hash<xmipp4::compute::memory_transfer_key>::operator()(
    const xmipp4::compute::memory_transfer_key& key
) const noexcept
{
    const hash<const xmipp4::compute::memory_resource*> subhasher;


    const auto h1 = subhasher(key.get_source());
    const auto h2 = subhasher(key.get_destination());

    return h1 ^ (h2 << 1);
}

} // namespace std
