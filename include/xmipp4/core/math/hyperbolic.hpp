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
 * @file hyperbolic.hpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Provides hyperbolic functions.
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
cosh(F angle) noexcept;

template <typename F>
typename std::enable_if<std::is_floating_point<F>::value, F>::type
sinh(F angle) noexcept;

template <typename F>
typename std::enable_if<std::is_floating_point<F>::value, F>::type
tanh(F angle) noexcept;

template <typename F>
typename std::enable_if<std::is_floating_point<F>::value, F>::type
acosh(F ratio) noexcept;

template <typename F>
typename std::enable_if<std::is_floating_point<F>::value, F>::type
asinh(F ratio) noexcept;

template <typename F>
typename std::enable_if<std::is_floating_point<F>::value, F>::type
atanh(F ratio) noexcept;

} // namespace math
} // namespace xmipp4

#include "hyperbolic.inl"
