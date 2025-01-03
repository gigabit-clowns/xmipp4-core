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
 * @file trigonometric.hpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Provides trigonometric functions.
 * @date 2024-04-15
 * 
 */

#include <utility>
#include <type_traits>

#include "../platform/constexpr.hpp"

namespace xmipp4
{
namespace math
{

template <typename F>
typename std::enable_if<std::is_floating_point<F>::value, F>::type
cos(F angle) noexcept;

template <typename F>
typename std::enable_if<std::is_floating_point<F>::value, F>::type
sin(F angle) noexcept;

template <typename F>
typename std::enable_if<std::is_floating_point<F>::value, void>::type
sincos(F angle, F &y, F &x) noexcept;

template <typename F>
typename std::enable_if<std::is_floating_point<F>::value, std::pair<F, F>>::type
sincos(F angle) noexcept;

template <typename F>
typename std::enable_if<std::is_floating_point<F>::value, F>::type
tan(F angle) noexcept;

template <typename F>
typename std::enable_if<std::is_floating_point<F>::value, F>::type
acos(F ratio) noexcept;

template <typename F>
typename std::enable_if<std::is_floating_point<F>::value, F>::type
asin(F ratio) noexcept;

template <typename F>
typename std::enable_if<std::is_floating_point<F>::value, F>::type
atan(F ratio) noexcept;

template <typename F>
typename std::enable_if<std::is_floating_point<F>::value, F>::type
atan2(F y, F x) noexcept;

template <typename I>
XMIPP4_CONSTEXPR
typename std::enable_if<std::is_integral<I>::value, int>::type
cos_pi(I x) noexcept;

} // namespace math
} // namespace xmipp4

#include "trigonometric.inl"
