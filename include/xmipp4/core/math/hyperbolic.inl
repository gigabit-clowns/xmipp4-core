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
 * @file hyperbolic.inl
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Implementation of hyperbolic.hpp
 * @date 2024-04-15
 * 
 */

#include "hyperbolic.hpp"

#include "../platform/builtin.h"

#include <cmath>

namespace xmipp4
{
namespace math
{

namespace detail
{

inline float cosh(float angle) noexcept
{
#if XMIPP4_HAS_BUILTIN(coshf)
    return XMIPP4_BUILTIN(coshf)(angle);
#else
    return coshf(angle);
#endif
}

inline double cosh(double angle) noexcept
{
#if XMIPP4_HAS_BUILTIN(cosh)
    return XMIPP4_BUILTIN(cosh)(angle);
#else
    return ::cosh(angle);
#endif
}

inline long double cosh(long double angle) noexcept
{
#if XMIPP4_HAS_BUILTIN(coshl)
    return XMIPP4_BUILTIN(coshl)(angle);
#else
    return coshl(angle);
#endif
}

} // namespace detail

template <typename F>
inline
typename std::enable_if<std::is_floating_point<F>::value, F>::type
cosh(F angle) noexcept
{
    return detail::cosh(angle);
}





namespace detail
{

inline float sinh(float angle) noexcept
{
#if XMIPP4_HAS_BUILTIN(sinhf)
    return XMIPP4_BUILTIN(sinhf)(angle);
#else
    return sinhf(angle);
#endif
}

inline double sinh(double angle) noexcept
{
#if XMIPP4_HAS_BUILTIN(sinh)
    return XMIPP4_BUILTIN(sinh)(angle);
#else
    return ::sinh(angle);
#endif
}

inline long double sinh(long double angle) noexcept
{
#if XMIPP4_HAS_BUILTIN(sinhl)
    return XMIPP4_BUILTIN(sinhl)(angle);
#else
    return sinhl(angle);
#endif
}

} // namespace detail

template <typename F>
inline
typename std::enable_if<std::is_floating_point<F>::value, F>::type
sinh(F angle) noexcept
{
    return detail::sinh(angle);
}





namespace detail
{

inline float tanh(float angle) noexcept
{
#if XMIPP4_HAS_BUILTIN(tanhf)
    return XMIPP4_BUILTIN(tanhf)(angle);
#else
    return tanhf(angle);
#endif
}

inline double tanh(double angle) noexcept
{
#if XMIPP4_HAS_BUILTIN(tanh)
    return XMIPP4_BUILTIN(tanh)(angle);
#else
    return ::tanh(angle);
#endif
}

inline long double tanh(long double angle) noexcept
{
#if XMIPP4_HAS_BUILTIN(tanhl)
    return XMIPP4_BUILTIN(tanhl)(angle);
#else
    return tanhl(angle);
#endif
}

} // namespace detail

template <typename F>
inline
typename std::enable_if<std::is_floating_point<F>::value, F>::type
tanh(F angle) noexcept
{
    return detail::tanh(angle);
}





namespace detail
{

inline float acosh(float ratio) noexcept
{
#if XMIPP4_HAS_BUILTIN(acoshf)
    return XMIPP4_BUILTIN(acoshf)(ratio);
#else
    return acoshf(ratio);
#endif
}

inline double acosh(double ratio) noexcept
{
#if XMIPP4_HAS_BUILTIN(acosh)
    return XMIPP4_BUILTIN(acosh)(ratio);
#else
    return ::acosh(ratio);
#endif
}

inline long double acosh(long double ratio) noexcept
{
#if XMIPP4_HAS_BUILTIN(acoshl)
    return XMIPP4_BUILTIN(acoshl)(ratio);
#else
    return acoshl(ratio);
#endif
}

} // namespace detail

template <typename F>
inline
typename std::enable_if<std::is_floating_point<F>::value, F>::type
acosh(F ratio) noexcept
{
    return detail::acosh(ratio);
}





namespace detail
{

inline float asinh(float ratio) noexcept
{
#if XMIPP4_HAS_BUILTIN(asinhf)
    return XMIPP4_BUILTIN(asinhf)(ratio);
#else
    return asinhf(ratio);
#endif
}

inline double asinh(double ratio) noexcept
{
#if XMIPP4_HAS_BUILTIN(asinh)
    return XMIPP4_BUILTIN(asinh)(ratio);
#else
    return ::asinh(ratio);
#endif
}

inline long double asinh(long double ratio) noexcept
{
#if XMIPP4_HAS_BUILTIN(asinhl)
    return XMIPP4_BUILTIN(asinhl)(ratio);
#else
    return asinhl(ratio);
#endif
}

} // namespace detail


template <typename F>
inline
typename std::enable_if<std::is_floating_point<F>::value, F>::type
asinh(F ratio) noexcept
{
    return detail::asinh(ratio);
}





namespace detail
{

inline float atanh(float ratio) noexcept
{
#if XMIPP4_HAS_BUILTIN(atanhf)
    return XMIPP4_BUILTIN(atanhf)(ratio);
#else
    return atanhf(ratio);
#endif
}

inline double atanh(double ratio) noexcept
{
#if XMIPP4_HAS_BUILTIN(atanh)
    return XMIPP4_BUILTIN(atanh)(ratio);
#else
    return ::atanh(ratio);
#endif
}

inline long double atanh(long double ratio) noexcept
{
#if XMIPP4_HAS_BUILTIN(atanhl)
    return XMIPP4_BUILTIN(atanhl)(ratio);
#else
    return atanhl(ratio);
#endif
}

} // namespace detail

template <typename F>
inline
typename std::enable_if<std::is_floating_point<F>::value, F>::type
atanh(F ratio) noexcept
{
    return detail::atanh(ratio);
}

} // namespace math
} // namespace xmipp4
