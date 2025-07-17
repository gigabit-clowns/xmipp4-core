// SPDX-License-Identifier: GPL-3.0-only

#include "power.hpp"

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
square(U x) noexcept
{
    return x*x;
}

template <typename I>
XMIPP4_INLINE_CONSTEXPR
typename std::make_unsigned<typename std::enable_if<std::is_integral<I>::value && std::is_signed<I>::value, I>::type>::type
square(I x) noexcept
{
    const auto u = abs(x);
    return square(u);
}

template <typename F>
XMIPP4_INLINE_CONSTEXPR
typename std::enable_if<std::is_floating_point<F>::value, F>::type
square(F x) noexcept
{
    return x*x;
}





namespace detail
{

inline float pow(float base, float exp) noexcept
{
#if XMIPP4_HAS_BUILTIN(powf)
    return XMIPP4_BUILTIN(powf)(base, exp);
#else
    return powf(base, exp);
#endif
}

inline double pow(double base, double exp) noexcept
{
#if XMIPP4_HAS_BUILTIN(pow)
    return XMIPP4_BUILTIN(pow)(base, exp);
#else
    return ::pow(base, exp);
#endif
}

inline long double pow(long double base, long double exp) noexcept
{
#if XMIPP4_HAS_BUILTIN(powl)
    return XMIPP4_BUILTIN(powl)(base, exp);
#else
    return powl(base, exp);
#endif
}

} // namespace detail

template <typename F>
inline
typename std::enable_if<std::is_floating_point<F>::value, F>::type
pow(F base, F exp) noexcept
{
    return detail::pow(base, exp);
}





namespace detail
{

inline float sqrt(float x) noexcept
{
#if XMIPP4_HAS_BUILTIN(sqrtf)
    return XMIPP4_BUILTIN(sqrtf)(x);
#else
    return sqrtf(x);
#endif
}

inline double sqrt(double x) noexcept
{
#if XMIPP4_HAS_BUILTIN(sqrt)
    return XMIPP4_BUILTIN(sqrt)(x);
#else
    return ::sqrt(x);
#endif
}

inline long double sqrt(long double x) noexcept
{
#if XMIPP4_HAS_BUILTIN(sqrtl)
    return XMIPP4_BUILTIN(sqrtl)(x);
#else
    return sqrtl(x);
#endif
}

} // namespace detail

template <typename F>
inline
typename std::enable_if<std::is_floating_point<F>::value, F>::type
sqrt(F x) noexcept
{
    return detail::sqrt(x);
}


template <typename F>
typename std::enable_if<std::is_floating_point<F>::value, F>::type
rsqrt(F x) noexcept
{
    return F(1) / sqrt(x);
}





namespace detail
{

inline float cbrt(float x) noexcept
{
#if XMIPP4_HAS_BUILTIN(cbrtf)
    return XMIPP4_BUILTIN(cbrtf)(x);
#else
    return cbrtf(x);
#endif
}

inline double cbrt(double x) noexcept
{
#if XMIPP4_HAS_BUILTIN(cbrt)
    return XMIPP4_BUILTIN(cbrt)(x);
#else
    return ::cbrt(x);
#endif
}

inline long double cbrt(long double x) noexcept
{
#if XMIPP4_HAS_BUILTIN(cbrtl)
    return XMIPP4_BUILTIN(cbrtl)(x);
#else
    return cbrtl(x);
#endif
}

} // namespace detail

template <typename F>
inline
typename std::enable_if<std::is_floating_point<F>::value, F>::type
cbrt(F x) noexcept
{
    return detail::cbrt(x);
}

} // namespace math
} // namespace xmipp4
