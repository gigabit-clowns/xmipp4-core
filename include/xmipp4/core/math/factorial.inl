// SPDX-License-Identifier: GPL-3.0-only

#include "abs.hpp"

#include "../platform/builtin.h"

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
