// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "slice.hpp"
#include "../platform/constexpr.hpp"

#include <ostream>
#include <type_traits>

namespace xmipp4 
{
namespace multidimensional
{

/**
 * @brief Tag to express the consumption of an arbitrary amount 
 * of axes until no slack is left on the right-hand dimensions.
 * 
 * NumPy equivalent of Ellipsis or "...".
 * 
 */
struct ellipsis_tag {};

XMIPP4_CONSTEXPR
bool operator==(ellipsis_tag lhs, ellipsis_tag rhs) noexcept;
XMIPP4_CONSTEXPR
bool operator!=(ellipsis_tag lhs, ellipsis_tag rhs) noexcept;

template <typename T>
std::basic_ostream<T>& operator<<(std::basic_ostream<T> &os, ellipsis_tag);

/**
 * @brief Construct an ellipsis_tag.
 * 
 * @return ellipsis_tag Newly constructed tag.
 */
XMIPP4_CONSTEXPR
ellipsis_tag ellipsis() noexcept;



/**
 * @brief Tag to express the creation a new axis of size one.
 * 
 * NumPy equivalent of numpy.newaxis() or "None".
 * 
 */
struct new_axis_tag {};

XMIPP4_CONSTEXPR
bool operator==(new_axis_tag, new_axis_tag) noexcept;
XMIPP4_CONSTEXPR
bool operator!=(new_axis_tag, new_axis_tag) noexcept;

template <typename T>
std::basic_ostream<T>& operator<<(std::basic_ostream<T> &os, new_axis_tag);

/**
 * @brief Construct a new_axis_tag.
 * 
 * @return new_axis_tag Newly constructed tag.
 */
XMIPP4_CONSTEXPR
new_axis_tag new_axis() noexcept;

} // namespace multidimensional
} // namespace xmipp4

#include "subscript_tags.inl"
