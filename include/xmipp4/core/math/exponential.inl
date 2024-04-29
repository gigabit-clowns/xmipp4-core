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
 * @file exponential.inl
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Implementation of exponential.hpp
 * @date 2024-04-15
 * 
 */

#include "exponential.hpp"

#include "power.hpp"
#include "../platform/builtin.h"

#include <cmath>

namespace xmipp4
{
namespace math
{

namespace detail
{

inline float exp(float exponent) noexcept
{
#if XMIPP4_HAS_BUILTIN(expf)
    return XMIPP4_BUILTIN(expf)(exponent);
#else
    return expf(exponent);
#endif
}

inline double exp(double exponent) noexcept
{
#if XMIPP4_HAS_BUILTIN(exp)
    return XMIPP4_BUILTIN(exp)(exponent);
#else
    return ::exp(exponent);
#endif
}

inline long double exp(long double exponent) noexcept
{
#if XMIPP4_HAS_BUILTIN(expl)
    return XMIPP4_BUILTIN(expl)(exponent);
#else
    return expl(exponent);
#endif
}

} // namespace detail

template <typename F>
inline
typename std::enable_if<std::is_floating_point<F>::value, F>::type
exp(F exponent) noexcept
{
    return detail::exp(exponent);
}





namespace detail
{

inline float exp2(float exponent) noexcept
{
#if XMIPP4_HAS_BUILTIN(exp2f)
    return XMIPP4_BUILTIN(exp2f)(exponent);
#else
    return exp2f(exponent);
#endif
}

inline double exp2(double exponent) noexcept
{
#if XMIPP4_HAS_BUILTIN(exp2)
    return XMIPP4_BUILTIN(exp2)(exponent);
#else
    return ::exp2(exponent);
#endif
}

inline long double exp2(long double exponent) noexcept
{
#if XMIPP4_HAS_BUILTIN(exp2l)
    return XMIPP4_BUILTIN(exp2l)(exponent);
#else
    return exp2l(exponent);
#endif
}

} // namespace detail

template <typename F>
inline
typename std::enable_if<std::is_floating_point<F>::value, F>::type
exp2(F exponent) noexcept
{
    return detail::exp2(exponent);
}





namespace detail
{

template <typename F>
inline
typename std::enable_if<std::is_floating_point<F>::value, F>::type
exp10(F exponent) noexcept
{
    XMIPP4_CONST_CONSTEXPR F base = 10;
    return pow(base, exponent);
}

#if !defined(__APPLE__) //FIXME: Not compiling with conda, undefiend reference to _exp10
#if XMIPP4_HAS_BUILTIN(exp10f)
inline float exp10(float exponent) noexcept
{
    return XMIPP4_BUILTIN(exp10f)(exponent);
}
#endif

#if XMIPP4_HAS_BUILTIN(exp10)
inline double exp10(double exponent) noexcept
{
    return XMIPP4_BUILTIN(exp10)(exponent);
}
#endif

#if XMIPP4_HAS_BUILTIN(exp10l)
inline long double exp10(long double exponent) noexcept
{
    return XMIPP4_BUILTIN(exp10l)(exponent);
}
#endif
#endif

} // namespace detail

template <typename F>
inline
typename std::enable_if<std::is_floating_point<F>::value, F>::type
exp10(F exponent) noexcept
{
    return detail::exp10(exponent);
}





namespace detail
{

inline float log(float x) noexcept
{
#if XMIPP4_HAS_BUILTIN(logf)
    return XMIPP4_BUILTIN(logf)(x);
#else
    return logf(x);
#endif
}

inline double log(double x) noexcept
{
#if XMIPP4_HAS_BUILTIN(log)
    return XMIPP4_BUILTIN(log)(x);
#else
    return ::log(x);
#endif
}

inline long double log(long double x) noexcept
{
#if XMIPP4_HAS_BUILTIN(logl)
    return XMIPP4_BUILTIN(logl)(x);
#else
    return logl(x);
#endif
}

} // namespace detail

template <typename F>
inline
typename std::enable_if<std::is_floating_point<F>::value, F>::type
log(F x) noexcept
{
    return detail::log(x);
}





namespace detail
{

inline float log2(float x) noexcept
{
#if XMIPP4_HAS_BUILTIN(log2f)
    return XMIPP4_BUILTIN(log2f)(x);
#else
    return log2f(x);
#endif
}

inline double log2(double x) noexcept
{
#if XMIPP4_HAS_BUILTIN(log2)
    return XMIPP4_BUILTIN(log2)(x);
#else
    return ::log2(x);
#endif
}

inline long double log2(long double x) noexcept
{
#if XMIPP4_HAS_BUILTIN(log2l)
    return XMIPP4_BUILTIN(log2l)(x);
#else
    return log2l(x);
#endif
}

} // namespace detail

template <typename F>
inline
typename std::enable_if<std::is_floating_point<F>::value, F>::type
log2(F x) noexcept
{
    return detail::log2(x);
}





namespace detail
{

inline float log10(float x) noexcept
{
#if XMIPP4_HAS_BUILTIN(log10f)
    return XMIPP4_BUILTIN(log10f)(x);
#else
    return log10f(x);
#endif
}

inline double log10(double x) noexcept
{
#if XMIPP4_HAS_BUILTIN(log10)
    return XMIPP4_BUILTIN(log10)(x);
#else
    return ::log10(x);
#endif
}

inline long double log10(long double x) noexcept
{
#if XMIPP4_HAS_BUILTIN(log10l)
    return XMIPP4_BUILTIN(log10l)(x);
#else
    return log10l(x);
#endif
}

} // namespace detail

template <typename F>
inline
typename std::enable_if<std::is_floating_point<F>::value, F>::type
log10(F x) noexcept
{
    return detail::log10(x);
}





template <typename F>
inline
typename std::enable_if<std::is_floating_point<F>::value, F>::type
logn(F n, F x) noexcept
{
    return log(x) / log(n);
}

} // namespace math
} // namespace xmipp4
