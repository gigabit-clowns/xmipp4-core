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
 * @file nearest_integer.inl
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Implementation of nearest_integer.hpp
 * @date 2024-04-15
 * 
 */

#include "nearest_integer.hpp"

#include "../platform/builtin.h"

#include <cmath>

namespace xmipp4
{
namespace math
{

namespace detail
{

inline float trunc(float x) noexcept
{
#if XMIPP4_HAS_BUILTIN(truncf)
    return XMIPP4_BUILTIN(truncf)(x);
#else
    return truncf(x);
#endif
}

inline double trunc(double x) noexcept
{
#if XMIPP4_HAS_BUILTIN(trunc)
    return XMIPP4_BUILTIN(trunc)(x);
#else
    return ::trunc(x);
#endif
}

inline long double trunc(long double x) noexcept
{
#if XMIPP4_HAS_BUILTIN(truncl)
    return XMIPP4_BUILTIN(truncl)(x);
#else
    return truncl(x);
#endif
}

} // namespace detail

template <typename F>
inline
typename std::enable_if<std::is_floating_point<F>::value, F>::type
trunc(F x) noexcept
{
    return detail::trunc(x);
}





namespace detail
{

inline float floor(float x) noexcept
{
#if XMIPP4_HAS_BUILTIN(floorf)
    return XMIPP4_BUILTIN(floorf)(x);
#else
    return floorf(x);
#endif
}

inline double floor(double x) noexcept
{
#if XMIPP4_HAS_BUILTIN(floor)
    return XMIPP4_BUILTIN(floor)(x);
#else
    return ::floor(x);
#endif
}

inline long double floor(long double x) noexcept
{
#if XMIPP4_HAS_BUILTIN(floorl)
    return XMIPP4_BUILTIN(floorl)(x);
#else
    return floorl(x);
#endif
}

} // namespace detail

template <typename F>
inline
typename std::enable_if<std::is_floating_point<F>::value, F>::type
floor(F x) noexcept
{
    return detail::floor(x);
}





namespace detail
{

inline float ceil(float x) noexcept
{
#if XMIPP4_HAS_BUILTIN(ceilf)
    return XMIPP4_BUILTIN(ceilf)(x);
#else
    return ceilf(x);
#endif
}

inline double ceil(double x) noexcept
{
#if XMIPP4_HAS_BUILTIN(ceil)
    return XMIPP4_BUILTIN(ceil)(x);
#else
    return ::ceil(x);
#endif
}

inline long double ceil(long double x) noexcept
{
#if XMIPP4_HAS_BUILTIN(ceill)
    return XMIPP4_BUILTIN(ceill)(x);
#else
    return ceill(x);
#endif
}

} // namespace detail

template <typename F>
inline
typename std::enable_if<std::is_floating_point<F>::value, F>::type
ceil(F x) noexcept
{
    return detail::ceil(x);
}





namespace detail
{

inline float round(float x) noexcept
{
#if XMIPP4_HAS_BUILTIN(roundf)
    return XMIPP4_BUILTIN(roundf)(x);
#else
    return roundf(x);
#endif
}

inline double round(double x) noexcept
{
#if XMIPP4_HAS_BUILTIN(round)
    return XMIPP4_BUILTIN(round)(x);
#else
    return ::round(x);
#endif
}

inline long double round(long double x) noexcept
{
#if XMIPP4_HAS_BUILTIN(roundl)
    return XMIPP4_BUILTIN(roundl)(x);
#else
    return roundl(x);
#endif
}

} // namespace detail

template <typename F>
inline
typename std::enable_if<std::is_floating_point<F>::value, F>::type
round(F x) noexcept
{
    return detail::round(x);
}

} // namespace math
} // namespace xmipp4
