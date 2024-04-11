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

#include "../platform/builtin.h"

#include <cmath>

namespace xmipp4
{
namespace math
{

template <typename U>
XMIPP4_INLINE_CONSTEXPR
typename std::enable_if<std::is_unsigned<U>::value, U>::type
abs(U x) noexcept
{
    return x; // Nothing to do
}

template <typename I>
XMIPP4_INLINE_CONSTEXPR
typename std::enable_if<std::is_integral<I>::value && std::is_integral<I>::value, typename std::make_unsigned<I>::type>::type
abs(I x) noexcept
{
    return x < 0 ? -x : x;
}

namespace detail
{

inline float abs(float x) noexcept
{
#if XMIPP4_HAS_BUILTIN(fabsf)
    return XMIPP4_BUILTIN(fabsf)(x);
#else
    return fabsf(x);
#endif
}

inline double abs(double x) noexcept
{
#if XMIPP4_HAS_BUILTIN(fabs)
    return XMIPP4_BUILTIN(fabs)(x);
#else
    return fabs(x);
#endif
}

inline long double abs(long double x) noexcept
{
#if XMIPP4_HAS_BUILTIN(fabsl)
    return XMIPP4_BUILTIN(fabsl)(x);
#else
    return fabsl(x);
#endif
}

} // namespace detail

template <typename F>
inline
typename std::enable_if<std::is_floating_point<F>::value, F>::type
abs(F x) noexcept
{
    return detail::abs(x);
}

} // namespace math
} // namespace xmipp4
