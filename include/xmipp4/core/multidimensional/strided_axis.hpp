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
 * @file strided_axis.hpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Defines strided_axis class
 * @date 2023-08-13
 * 
 */

#include "slice.hpp"
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
class strided_axis 
{
public:
    /**
     * @brief Constructor
     * 
     * @param extent Number of elements in the axis. Defaults to 0
     * @param stride Step between consecutive elements. In items.
     */
    XMIPP4_CONSTEXPR strided_axis(std::size_t extent, 
                                  std::ptrdiff_t stride ) noexcept;

    strided_axis() = default;
    strided_axis(const strided_axis& other) = default;
    strided_axis(strided_axis&& other) = default;
    ~strided_axis() = default;

    strided_axis& operator=(const strided_axis& other) = default;
    strided_axis& operator=(strided_axis&& other) = default;
    
    XMIPP4_CONSTEXPR bool operator==(const strided_axis& other) const noexcept;
    XMIPP4_CONSTEXPR bool operator!=(const strided_axis& other) const noexcept;

    XMIPP4_CONSTEXPR_CPP20 void swap(strided_axis &other) noexcept;

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
    XMIPP4_CONSTEXPR std::size_t get_stride_magnitude() const noexcept;

private:
    std::size_t m_extent; ///< Number of elements
    std::ptrdiff_t m_stride; ///< Step between adjacent elements. In items

};

XMIPP4_CONSTEXPR_CPP20 void swap(strided_axis &x, strided_axis &y) noexcept;

/**
 * @brief Create a contiguous strided_axis.
 * 
 * A contiguous axis descriptor has a unitary stride.
 * 
 * @param extent Number of elements on the axis.
 * @return strided_axis. The resulting axis
 */
XMIPP4_CONSTEXPR
strided_axis make_contiguous_axis(std::size_t extent=1) noexcept;

/**
 * @brief Create a phantom strided_axis.
 * 
 * A phantom axis descriptor has null stride. 
 * This means that it does not contribute to the
 * underlying array's storage size and all its 
 * elements are repeated. 
 * 
 * @param extent Number of elements on the axis.
 * @return strided_axis. The resulting axis
 */
XMIPP4_CONSTEXPR
strided_axis make_phantom_axis(std::size_t extent=1) noexcept;

/**
 * @brief Compare the absolute strides of a given pair of axes.
 * 
 * @param lhs Left hand side operand.
 * @param rhs Right hand side operand.
 * @return bool True if left hand side's absolute stride is equal to 
 * right hand side's stride.
 */
XMIPP4_CONSTEXPR 
bool compare_strides_equal(const strided_axis &lhs, 
                           const strided_axis &rhs ) noexcept;

/**
 * @brief Compare the absolute strides of a given pair of axes.
 * 
 * @param lhs Left hand side operand.
 * @param rhs Right hand side operand.
 * @return bool True if left hand side's absolute stride is less than 
 * right hand side's stride.
 */
XMIPP4_CONSTEXPR 
bool compare_strides_less(const strided_axis &lhs, 
                          const strided_axis &rhs ) noexcept;

/**
 * @brief Compare the absolute strides of a given pair of axes.
 * 
 * @param lhs Left hand side operand.
 * @param rhs Right hand side operand.
 * @return bool True if left hand side's absolute stride is greater than 
 * right hand side's stride.
 */
XMIPP4_CONSTEXPR 
bool compare_strides_greater(const strided_axis &lhs, 
                             const strided_axis &rhs ) noexcept;

/**
 * @brief Check if an axis has a non zero stride.
 * 
 * @param axis The axis to be checked.
 * @return bool True if the axis has a non-zero stride.
 */
XMIPP4_CONSTEXPR
bool check_nonzero_stride(const strided_axis &axis) noexcept;

/**
 * @brief Check if an axis is contiguous.
 * 
 * An axis is contiguous if its stride is one.
 * 
 * @param axis Axis to be checked.
 * @return true if the axis is contiguous.
 * @return false if the axis is not contiguous.
 */
XMIPP4_CONSTEXPR
bool is_contiguous(const strided_axis &axis) noexcept;

/**
 * @brief Check if an axis pair is contiguous.
 * 
 * A pair of axes is contiguous if stride*extent of the
 * major axis is equal to the stride of the minor axis.
 * 
 * @param major Fast axis (smallest stride).
 * @param minor Slow axis (largest stride).
 * @return true if the axes are contiguous.
 * @return false if the axes are not contiguous.
 */
XMIPP4_CONSTEXPR
bool is_contiguous(const strided_axis &major,
                   const strided_axis &minor ) noexcept;

/**
 * @brief Check if an axis is contiguous also considering its mirror.
 * 
 * An axis is mirror contiguous if the magnitude of its stride is one.
 * 
 * @param axis Axis to be checked.
 * @return true if the axis is mirror contiguous.
 * @return false if the axis is not mirror contiguous.
 */
XMIPP4_CONSTEXPR
bool is_mirror_contiguous(const strided_axis &axis) noexcept;
/**
 * @brief Check if an axis pair is contiguous also considering their
 * mirrors.
 * 
 * A pair of axes is contiguous if abs(stride)*extent of the
 * major axis is equal to the abs(stride) of the minor axis.
 * 
 * @param major Fast axis (smallest stride magnitude).
 * @param minor Slow axis (largest stride magnitude).
 * @return true if the axes are mirror contiguous.
 * @return false if the axes are not mirror contiguous.
 */
XMIPP4_CONSTEXPR
bool is_mirror_contiguous(const strided_axis &major,
                          const strided_axis &minor ) noexcept;

/**
 * @brief Check if an axis is reversed.
 * 
 * An axis is reversed if has negative stride.
 * 
 * @param axis Axis to be checked.
 * @return bool True if the axis is reversed.
 */
XMIPP4_CONSTEXPR
bool is_reversed(const strided_axis &axis) noexcept;

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
bool is_repeating(const strided_axis &axis) noexcept;

/**
 * @brief Check if an axis is empty
 * 
 * An axis is empty if it has an extent of zero.
 * 
 * @param axis The axis to be checked
 * @return true if the axis is empty
 * @return false if the axis is not empty.
 */
XMIPP4_CONSTEXPR
bool is_empty(const strided_axis &axis) noexcept;

/**
 * @brief Check if an axis is significant.
 * 
 * An axis is significant if it has an extent not equal to 1.
 * 
 * @param axis The axis to be checked.
 * @return true if the axis is significant.
 * @return false if the axis is not significant.
 */
XMIPP4_CONSTEXPR
bool is_significant(const strided_axis &axis) noexcept;

/**
 * @brief Check if an axis can be squeezed.
 * 
 * An axis can be squeezed only if it is not significant. This is,
 * if it has an extent equal to 1.
 * 
 * @param axis The axis to be checked
 * @return true If the axis is not significant
 * @return false If the axis is significant.
 * @see is_significant
 * 
 */
XMIPP4_CONSTEXPR 
bool check_squeeze(const strided_axis &axis) noexcept;

/**
 * @brief Get the axis last position referenced by ab axis.
 * 
 * @param axis The last position referenced by the axis.
 * @param result Output variable with the result. Only written when the axis
 * has non-zero extent.
 * @return true if the axis has non-zero extent and output was written.
 * @return false if the axis has an extent of zero.
 */
XMIPP4_CONSTEXPR 
bool get_axis_last_offset(const strided_axis &axis, 
                          std::ptrdiff_t &result) noexcept;

/**
 * @brief Obtain the offset applied when reversing an axis.
 * 
 * If the axis is empty, this function returns 0 regardless of the stride.
 * If the axis is not reversed, this function returns 0.
 * If the axis is reversed, this function returns (extent-1)*unsigned_stride
 * of the axis.
 * 
 * @param axis Axis to be analyzed. 
 * @return std::size_t The offset.
*/
XMIPP4_CONSTEXPR
std::size_t get_axis_pivot_offset(const strided_axis &axis) noexcept;

/**
 * @brief Perform a broadcast between an axis an a extent.
 * 
 * If the axis' extent matches the provided extent, nothing is modified
 * and true is returned.
 * If the axis' extent does not match the provided extent but the former 
 * one is 1, the axis is replaced a phantom axis with an extent of the 
 * later value and true is returned.
 * If the axis' extent does not match the provided extent but this equals 1,
 * it will be overwritten with the extent of the axis and true is returned.
 * If the extent mismatch and none of them equals 1, nothing is modified and
 * false is returned. 
 * 
 * @param axis1 The axis to be broadcasted.
 * @param extent2 The extent to be broadcasted.
 * @return true When successful.
 * @return false when unable to broadcast.
 * 
 */
XMIPP4_CONSTEXPR
bool broadcast(strided_axis &axis, std::size_t &extent) noexcept;

/**
 * @brief Perform a broadcast of an axis to a specific extent.
 * 
 * If the axis' extent matches the provided extent, nothing is modified
 * and true is returned.
 * If the axis' extent does not match the provided extent but the former 
 * one is 1, the axis is replaced with a phantom axis with the provided 
 * extent, and true is returned.
 * If the axis' extent does not match the provided extent and is not 1, 
 * nothing is modified and false is returned.
 * 
 * @param axis The axis to be broadcasted.
 * @param extent The target extent for broadcasting.
 * @return true When successful.
 * @return false When unable to broadcast.
 */
XMIPP4_CONSTEXPR
bool broadcast_to(strided_axis &axis, std::size_t extent) noexcept;

/**
 * @brief Apply an index to an axis descriptor to increment the offset
 * 
 * @tparam I Index type.
 * @param desc Axis descriptor.
 * @param index The index.
 * @param offset Offset to be incremented.
 */
template <typename I>
void apply_index(const strided_axis &desc,
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
 * @param desc Axis descriptor. Will be modified according to the 
 * provided slice.
 * @param s Slice to be applied.
 * @param offset Offset to be incremented.
 * 
 */
template <typename Start, typename Stop, typename Step>
void apply_slice(strided_axis &desc, 
                 const slice<Start, Stop, Step> &s,
                 std::ptrdiff_t &offset );

} // namespace multidimensional
} // namespace xmipp4

#include "strided_axis.inl"
