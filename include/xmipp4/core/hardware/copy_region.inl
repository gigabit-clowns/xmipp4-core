// SPDX-License-Identifier: GPL-3.0-only

#include "copy_region.hpp"

namespace xmipp4 
{
namespace hardware
{

XMIPP4_INLINE_CONSTEXPR 
copy_region::copy_region() noexcept
    : m_source_offset(0)
    , m_destination_offset(0)
    , m_count(0)
{
}

XMIPP4_INLINE_CONSTEXPR 
copy_region::copy_region(std::size_t source_offset, 
                         std::size_t destination_offset,
                         std::size_t count ) noexcept
    : m_source_offset(source_offset)
    , m_destination_offset(destination_offset)
    , m_count(count)
{
}


XMIPP4_INLINE_CONSTEXPR
std::size_t copy_region::get_source_offset() const noexcept
{
    return m_source_offset;
}

XMIPP4_INLINE_CONSTEXPR
std::size_t copy_region::get_destination_offset() const noexcept
{
    return m_destination_offset;
}

XMIPP4_INLINE_CONSTEXPR
std::size_t copy_region::get_count() const noexcept
{
    return m_count;
}

} // namespace hardware
} // namespace xmipp4
