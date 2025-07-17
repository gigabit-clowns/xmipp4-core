// SPDX-License-Identifier: GPL-3.0-only

#pragma once

/**
 * @file nearest_integer.hpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Provides nearest integer rounding functions.
 * @date 2024-04-15
 * 
 */

#include <type_traits>

namespace xmipp4
{
namespace math
{

template <typename F>
typename std::enable_if<std::is_floating_point<F>::value, F>::type
trunc(F x) noexcept;

template <typename F>
typename std::enable_if<std::is_floating_point<F>::value, F>::type
floor(F x) noexcept;

template <typename F>
typename std::enable_if<std::is_floating_point<F>::value, F>::type
ceil(F x) noexcept;

template <typename F>
typename std::enable_if<std::is_floating_point<F>::value, F>::type
round(F x) noexcept;

} // namespace math
} // namespace xmipp4

#include "nearest_integer.inl"
