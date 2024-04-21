#pragma once

/***************************************************************************
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
 * 02111-1307  USA
 *
 *  All comments concerning this program package may be sent to the
 *  e-mail address 'xmipp@cnb.csic.es'
 ***************************************************************************/

/**
 * @file axis_descriptor.hpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Defines axis_descriptor class
 * @date 2023-08-13
 * 
 */

#include "../slice.hpp"
#include "../platform/constexpr.hpp"
#include "../platform/attributes.hpp"

#include <cstddef>

namespace xmipp4 
{
namespace multidimensional
{

/**
 * @brief Class describing element layout on a tensor
 * axis
 * 
 */
class axis_descriptor 
{
public:
    /**
     * @brief Constructor
     * 
     * @param extent Number of elements in the axis. Defaults to 0
     * @param stride Step between consecutive elements. In items. Defaults to 1
     */
    XMIPP4_CONSTEXPR explicit axis_descriptor(std::size_t extent = 0, 
                                              std::ptrdiff_t stride = 1 ) noexcept;

    axis_descriptor(const axis_descriptor& other) = default;
    axis_descriptor(axis_descriptor&& other) = default;
    ~axis_descriptor() = default;

    axis_descriptor& operator=(const axis_descriptor& other) = default;
    axis_descriptor& operator=(axis_descriptor&& other) = default;
    
    XMIPP4_CONSTEXPR bool operator==(const axis_descriptor& other) const noexcept;
    XMIPP4_CONSTEXPR bool operator!=(const axis_descriptor& other) const noexcept;

    XMIPP4_CONSTEXPR_CPP20 void swap(axis_descriptor &other) noexcept;

    /**
     * @brief Set the element count
     * 
     * @param extent The element count
     */
    XMIPP4_CONSTEXPR void set_extent(std::size_t extent) noexcept;

    /**
     * @brief Return the element count
     * 
     * @return std::size_t Element count
     */
    XMIPP4_CONSTEXPR std::size_t get_extent() const noexcept;

    /**
     * @brief Set the stride between consecutive elements
     * 
     * @param stride Step between consecutive elements. In items
     */
    XMIPP4_CONSTEXPR void set_stride(std::ptrdiff_t stride) noexcept;

    /**
     * @brief Get the stride between consecutive elements
     * 
     * @return std::ptrdiff_t Step between consecutive elements. In items
     */
    XMIPP4_CONSTEXPR std::ptrdiff_t get_stride() const noexcept;

    /**
     * @brief Get the unsigned stride between consecutive elements
     * 
     * @return std::size_t Step between consecutive elements. In items
     */
    XMIPP4_CONSTEXPR std::size_t get_unsigned_stride() const noexcept;

    /**
     * @brief Get the total size of the axis. In items
     * 
     * @return std::size_t Axis size. In items
     */
    XMIPP4_CONSTEXPR std::size_t get_width() const noexcept;

private:
    std::size_t m_extent; ///< Number of elements
    std::ptrdiff_t m_stride; ///< Step between adjacent elements. In items

};

/**
 * @brief Create a contiguous axis_descriptor.
 * 
 * A contiguous axis descriptor has a unitary stride.
 * 
 * @param extent Number of elements on the axis.
 * @return axis_descriptor. The resulting axis
 */
XMIPP4_CONSTEXPR
axis_descriptor make_contiguous_axis(std::size_t extent=1) noexcept;

/**
 * @brief Create a phantom axis_descriptor.
 * 
 * A phantom axis descriptor has null stride. 
 * This means that it does not contribute to the
 * underlying array's size and all its elements
 * are repeated. 
 * 
 * @param extent Number of elements on the axis.
 * @return axis_descriptor. The resulting axis
 */
XMIPP4_CONSTEXPR
axis_descriptor make_phantom_axis(std::size_t extent=1) noexcept;

XMIPP4_CONSTEXPR_CPP20 void swap(axis_descriptor &x, axis_descriptor &y) noexcept;



template <typename I>
void apply_index(const axis_descriptor &desc,
                 I index,
                 std::ptrdiff_t &offset );

template <typename Start, typename Stride, typename Stop>
axis_descriptor apply_slice(const axis_descriptor &desc, 
                            const slice<Start, Stride, Stop> &s,
                            std::ptrdiff_t &offset );

} // namespace multidimensional
} // namespace xmipp4

#include "axis_descriptor.inl"