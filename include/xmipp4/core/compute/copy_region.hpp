// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "../platform/constexpr.hpp"

#include <cstddef>

namespace xmipp4 
{
namespace compute
{

/**
 * @brief Represents a region in a buffer to be copied. 
 * 
 * This is expressed in terms of an offset in the source and destination
 * buffers together with an element count. 
 * 
 * @note Unless otherwise specified, these offsets and counts 
 * represent in bytes.
 * 
 */
class copy_region
{
public:
    XMIPP4_CONSTEXPR copy_region() noexcept;
    XMIPP4_CONSTEXPR
    copy_region(std::size_t source_offset, 
                std::size_t destination_offset,
                std::size_t count ) noexcept;
    copy_region(const copy_region &other) = default;
    copy_region(copy_region &&other) = default;
    ~copy_region() = default;

    copy_region& operator=(const copy_region &other) = default;
    copy_region& operator=(copy_region &&other) = default;

    XMIPP4_CONSTEXPR std::size_t get_source_offset() const noexcept;
    XMIPP4_CONSTEXPR std::size_t get_destination_offset() const noexcept;
    XMIPP4_CONSTEXPR std::size_t get_count() const noexcept;

private:
    std::size_t m_source_offset;
    std::size_t m_destination_offset;
    std::size_t m_count;

};

} // namespace compute
} // namespace xmipp4

#include "copy_region.inl"
