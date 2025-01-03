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
    XMIPP4_CONSTEXPR axis_descriptor(std::size_t extent, 
                                     std::ptrdiff_t stride ) noexcept;

    axis_descriptor() = default;
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
     * @brief Get the stride magnitude.
     * 
     * @return std::size_t Step between consecutive elements. In items
     */
    XMIPP4_CONSTEXPR std::size_t get_unsigned_stride() const noexcept;

private:
    std::size_t m_extent; ///< Number of elements
    std::ptrdiff_t m_stride; ///< Step between adjacent elements. In items

};

XMIPP4_CONSTEXPR_CPP20 void swap(axis_descriptor &x, axis_descriptor &y) noexcept;

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


/**
 * @brief Compute the last position referenced by an axis.
 * 
 * This is equivalent to (extent-1)*abs(stride) when extent is not zero.
 * If the axis has an extent of zero, -1 is returned.
 * 
 * @param axis The axis.
 * @return std::ptrdiff_t Axis last position. -1 if the axis has a zero
 * extent.
 */
XMIPP4_CONSTEXPR 
std::ptrdiff_t get_axis_last_position(const axis_descriptor &axis) noexcept;

/**
 * @brief Compute the product of the stride magnitude and extent
 * 
 * @note This does not need to be confused with the number of elements 
 * referenced by the axis. To do so, please refer to get_extent
 * @param axis The axis.
 * @return std::size_t The product of the stride magnitude and extent.
 */
XMIPP4_CONSTEXPR 
std::size_t get_axis_length(const axis_descriptor &axis) noexcept;

/**
 * @brief Compare the absolute strides of a given pair of axes.
 * 
 * @param lhs Left hand side operand.
 * @param rhs Right hand side operand.
 * @return bool True if left hand side's absolute stride is equal to 
 * right hand side's stride.
 */
XMIPP4_CONSTEXPR 
bool compare_strides_equal(const axis_descriptor &lhs, 
                           const axis_descriptor &rhs ) noexcept;

/**
 * @brief Compare the absolute strides of a given pair of axes.
 * 
 * @param lhs Left hand side operand.
 * @param rhs Right hand side operand.
 * @return bool True if left hand side's absolute stride is less than 
 * right hand side's stride.
 */
XMIPP4_CONSTEXPR 
bool compare_strides_less(const axis_descriptor &lhs, 
                          const axis_descriptor &rhs ) noexcept;

/**
 * @brief Compare the absolute strides of a given pair of axes.
 * 
 * @param lhs Left hand side operand.
 * @param rhs Right hand side operand.
 * @return bool True if left hand side's absolute stride is greater than 
 * right hand side's stride.
 */
XMIPP4_CONSTEXPR 
bool compare_strides_greater(const axis_descriptor &lhs, 
                             const axis_descriptor &rhs ) noexcept;

/**
 * @brief Check if an axis has a non zero stride.
 * 
 * @param axis The axis to be checked.
 * @return bool True if the axis has a non-zero stride.
 */
XMIPP4_CONSTEXPR
bool check_nonzero_stride(const axis_descriptor &axis) noexcept;

/**
 * @brief Check if an axis is contiguous.
 * 
 * An axis is contiguous if the magnitude of its stride
 * is one.
 * 
 * @param axis Axis to be checked.
 * @return bool True if the axis is contiguous.
 */
XMIPP4_CONSTEXPR
bool is_contiguous(const axis_descriptor &axis) noexcept;

/**
 * @brief Check if an axis pair is regular.
 * 
 * A pair of axes is regular if abs(stride)*extent of the
 * major axis is equal to the abs(stride) of the minor axis.
 * 
 * @param major Fast axis (smallest stride).
 * @param minor Slow axis (largest stride).
 * @return bool True if the axes are regular.
 */
XMIPP4_CONSTEXPR
bool is_regular(const axis_descriptor &major,
                const axis_descriptor &minor ) noexcept;

/**
 * @brief Check if an axis is reversed.
 * 
 * An axis is reversed if has negative stride.
 * 
 * @param axis Axis to be checked.
 * @return bool True if the axis is reversed.
 */
XMIPP4_CONSTEXPR
bool is_reversed(const axis_descriptor &axis) noexcept;

/**
 * @brief Check if an axis is significant.
 * 
 * An axis is significant if it has a non-zero stride
 * and an extent unequal to 1.
 * 
 * @param axis Axis to be checked.
 * @return bool True if the axis is significant.
 */
XMIPP4_CONSTEXPR
bool is_significant(const axis_descriptor &axis) noexcept;

/**
 * @brief Check if an axis is repeating.
 * 
 * An axis is repeating if it has an extent greater than one
 * and zero stride.
 * 
 * @param axis Axis to be checked.
 * @return bool True if the axis is repeating.
 */
XMIPP4_CONSTEXPR
bool is_repeating(const axis_descriptor &axis) noexcept;

/**
 * @brief Obtain the offset applied when reversing an axis.
 * 
 * If the axis is not reversed, this function returns true.
 * If the axis is reversed, this function returns (extent-1)*unsigned_stride
 * of the axis.
 * 
 * @param axis Axis to be analyzed. 
 * @return std::size_t The offset.
*/
XMIPP4_CONSTEXPR
std::size_t get_reverse_axis_offset(const axis_descriptor &axis) noexcept;

/**
 * @brief Check if an axis can be squeezed
 * 
 * An axis can be squeezed only if it has a extent of 1
 * 
 * @param axis The axis to be checked
 * @return bool True if the axis can be squeezed. False otherwise
 */
XMIPP4_CONSTEXPR 
bool check_squeeze(const axis_descriptor &axis) noexcept;

/**
 * @brief Broadcast axis extents.
 * 
 * Broadcasting tries to match axis extents without altering the
 * storage requirements. 
 * 
 * If all axes have the same extents nothing is modified and true 
 * is returned.
 * If some axes mismatch the size of the rest but have an extent of 1,
 * they are replaced with a phantom axis with the same extent as the other 
 * ones and true is returned. For this to happen, there needs to be at
 * least one axis with an extent different to 1.
 * Otherwise nothing can be performed to match axis extents and false 
 * is returned. In this case, axes are left in a valid but undefined state.
 * 
 * @tparam AxisDescriptors axis_descriptor types.
 * @param descriptors Axis descriptors to be broadcasted
 * @return bool True when success, False otherwise.
 */
template<typename... AxisDescriptors>
XMIPP4_CONSTEXPR
bool broadcast(AxisDescriptors&... descriptors) noexcept;

/**
 * @brief Apply an index to an axis descriptor to increment the offset
 * 
 * @tparam I Index type.
 * @param desc Axis descriptor.
 * @param index The index.
 * @param offset Offset to be incremented.
 */
template <typename I>
void apply_index(const axis_descriptor &desc,
                 I index,
                 std::ptrdiff_t &offset );

/**
 * @brief Apply a slice to an axis descriptor.
 * 
 * When slicing an axis, an offset can be introduced to the
 * current view. Additionally, axis' stride and extent may
 * be modified.
 * 
 * @tparam Start Start type.
 * @tparam Stop Stop type.
 * @tparam Step Step type.
 * @param desc Axis descriptor.
 * @param s Slice to be applied.
 * @param offset Offset to be incremented.
 * @return axis_descriptor Sliced axis descriptor.
 */
template <typename Start, typename Stop, typename Step>
axis_descriptor apply_slice(const axis_descriptor &desc, 
                            const slice<Start, Stop, Step> &s,
                            std::ptrdiff_t &offset );

} // namespace multidimensional
} // namespace xmipp4

#include "axis_descriptor.inl"
