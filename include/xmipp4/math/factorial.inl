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

#include "abs.hpp"

#include <cmath>

namespace xmipp4
{
namespace math
{

template <typename U>
XMIPP4_CONSTEXPR
typename std::enable_if<std::is_unsigned<U>::value, U>::type
factorial(U x) noexcept
{
    U result = 1;
    for(; x > 0; --x) result *= x;
    return result;
}

template <typename T, typename U>
XMIPP4_INLINE_CONSTEXPR
typename std::enable_if<std::is_unsigned<U>::value, T>::type
large_factorial(U x) noexcept
{
    T result = 1;
    for (U i = 2; i <= x; ++i) result *= i;
    return result;
}





namespace detail
{

inline float gamma(float x) noexcept
{
#if XMIPP4_HAS_BUILTIN(tgammaf)
    return XMIPP4_BUILTIN(tgammaf)(x);
#else
    return tgammaf(x);
#endif
}

inline double gamma(double x) noexcept
{
#if XMIPP4_HAS_BUILTIN(tgamma)
    return XMIPP4_BUILTIN(tgamma)(x);
#else
    return tgamma(x);
#endif
}

inline long double gamma(long double x) noexcept
{
#if XMIPP4_HAS_BUILTIN(tgammal)
    return XMIPP4_BUILTIN(tgammal)(x);
#else
    return tgammal(x);
#endif
}

} // namespace detail

template <typename F>
inline
typename std::enable_if<std::is_floating_point<F>::value, F>::type
gamma(F x) noexcept
{
    return detail::gamma(x);
}

} // namespace math
} // namespace xmipp4
