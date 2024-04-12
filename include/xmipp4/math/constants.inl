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

#include "constants.hpp"

namespace xmipp4
{
namespace math
{

template <typename F>
XMIPP4_INLINE_CONSTEXPR
typename std::enable_if<std::is_floating_point<F>::value, F>::type
pi() noexcept
{
    return 3.14159265358979323846L;
}

template <typename F>
XMIPP4_INLINE_CONSTEXPR
typename std::enable_if<std::is_floating_point<F>::value, F>::type
pi_2() noexcept
{
    return 1.57079632679489661923L;
}

template <typename F>
XMIPP4_INLINE_CONSTEXPR
typename std::enable_if<std::is_floating_point<F>::value, F>::type
pi_4() noexcept
{
    return 0.785398163397448309616L;
}

template <typename F>
XMIPP4_INLINE_CONSTEXPR
typename std::enable_if<std::is_floating_point<F>::value, F>::type
e() noexcept
{
    return 2.71828182845904523536L;
}

template <typename F>
XMIPP4_INLINE_CONSTEXPR
typename std::enable_if<std::is_floating_point<F>::value, F>::type
sqrt2() noexcept
{
    return 1.41421356237309504880L;
}

template <typename F>
XMIPP4_INLINE_CONSTEXPR
typename std::enable_if<std::is_floating_point<F>::value, F>::type
sqrt1_2() noexcept
{
    return 0.707106781186547524401L;
}

} // namespace math
} // namespace xmipp4
