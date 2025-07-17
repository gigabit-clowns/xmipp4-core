// SPDX-License-Identifier: GPL-3.0-only

#include "trigonometric.hpp"

#include "../platform/builtin.h"

#include <cmath>

namespace xmipp4
{
namespace math
{

namespace detail
{

inline float cos(float angle) noexcept
{
#if XMIPP4_HAS_BUILTIN(cosf)
    return XMIPP4_BUILTIN(cosf)(angle);
#else
    return cosf(angle);
#endif
}

inline double cos(double angle) noexcept
{
#if XMIPP4_HAS_BUILTIN(cos)
    return XMIPP4_BUILTIN(cos)(angle);
#else
    return ::cos(angle);
#endif
}

inline long double cos(long double angle) noexcept
{
#if XMIPP4_HAS_BUILTIN(cosl)
    return XMIPP4_BUILTIN(cosl)(angle);
#else
    return cosl(angle);
#endif
}

} // namespace detail

template <typename F>
inline
typename std::enable_if<std::is_floating_point<F>::value, F>::type
cos(F angle) noexcept
{
    return detail::cos(angle);
}





namespace detail
{

inline float sin(float angle) noexcept
{
#if XMIPP4_HAS_BUILTIN(sinf)
    return XMIPP4_BUILTIN(sinf)(angle);
#else
    return sinf(angle);
#endif
}

inline double sin(double angle) noexcept
{
#if XMIPP4_HAS_BUILTIN(sin)
    return XMIPP4_BUILTIN(sin)(angle);
#else
    return ::sin(angle);
#endif
}

inline long double sin(long double angle) noexcept
{
#if XMIPP4_HAS_BUILTIN(sinl)
    return XMIPP4_BUILTIN(sinl)(angle);
#else
    return sinl(angle);
#endif
}

} // namespace detail

template <typename F>
inline
typename std::enable_if<std::is_floating_point<F>::value, F>::type
sin(F angle) noexcept
{
    return detail::sin(angle);
}





namespace detail
{

template <typename F>
inline
typename std::enable_if<std::is_floating_point<F>::value, void>::type
sincos(F angle, F &y, F &x) noexcept
{
    y = sin(angle);
    x = cos(angle);
}

#if XMIPP4_HAS_BUILTIN(sincosf)
inline void sincos(float angle, float &y, float &x) noexcept
{
    XMIPP4_BUILTIN(sincosf)(angle, &y, &x);
}
#endif

#if XMIPP4_HAS_BUILTIN(sincos)
inline void sincos(double angle, double &y, double &x) noexcept
{
    XMIPP4_BUILTIN(sincos)(angle, &y, &x);
}
#endif

#if XMIPP4_HAS_BUILTIN(sincosl)
inline void sincos(long double angle, long double &y, long double &x) noexcept
{
    XMIPP4_BUILTIN(sincosl)(angle, &y, &x);
}
#endif

} // namespace detail

template <typename F>
inline
typename std::enable_if<std::is_floating_point<F>::value, void>::type
sincos(F angle, F &y, F &x) noexcept
{
    detail::sincos(angle, y, x);
}

template <typename F>
inline
typename std::enable_if<std::is_floating_point<F>::value, std::pair<F, F>>::type
sincos(F angle) noexcept
{
    std::pair<F, F> result;
    sincos(angle, result.first, result.second);
    return result;
}





namespace detail
{

inline float tan(float angle) noexcept
{
#if XMIPP4_HAS_BUILTIN(tanf)
    return XMIPP4_BUILTIN(tanf)(angle);
#else
    return tanf(angle);
#endif
}

inline double tan(double angle) noexcept
{
#if XMIPP4_HAS_BUILTIN(tan)
    return XMIPP4_BUILTIN(tan)(angle);
#else
    return ::tan(angle);
#endif
}

inline long double tan(long double angle) noexcept
{
#if XMIPP4_HAS_BUILTIN(tanl)
    return XMIPP4_BUILTIN(tanl)(angle);
#else
    return tanl(angle);
#endif
}

} // namespace detail

template <typename F>
inline
typename std::enable_if<std::is_floating_point<F>::value, F>::type
tan(F angle) noexcept
{
    return detail::tan(angle);
}





namespace detail
{

inline float acos(float ratio) noexcept
{
#if XMIPP4_HAS_BUILTIN(acosf)
    return XMIPP4_BUILTIN(acosf)(ratio);
#else
    return acosf(ratio);
#endif
}

inline double acos(double ratio) noexcept
{
#if XMIPP4_HAS_BUILTIN(acos)
    return XMIPP4_BUILTIN(acos)(ratio);
#else
    return ::acos(ratio);
#endif
}

inline long double acos(long double ratio) noexcept
{
#if XMIPP4_HAS_BUILTIN(acosl)
    return XMIPP4_BUILTIN(acosl)(ratio);
#else
    return acosl(ratio);
#endif
}

} // namespace detail

template <typename F>
inline
typename std::enable_if<std::is_floating_point<F>::value, F>::type
acos(F ratio) noexcept
{
    return detail::acos(ratio);
}





namespace detail
{

inline float asin(float ratio) noexcept
{
#if XMIPP4_HAS_BUILTIN(asinf)
    return XMIPP4_BUILTIN(asinf)(ratio);
#else
    return asinf(ratio);
#endif
}

inline double asin(double ratio) noexcept
{
#if XMIPP4_HAS_BUILTIN(asin)
    return XMIPP4_BUILTIN(asin)(ratio);
#else
    return ::asin(ratio);
#endif
}

inline long double asin(long double ratio) noexcept
{
#if XMIPP4_HAS_BUILTIN(asinl)
    return XMIPP4_BUILTIN(asinl)(ratio);
#else
    return asinl(ratio);
#endif
}

} // namespace detail


template <typename F>
inline
typename std::enable_if<std::is_floating_point<F>::value, F>::type
asin(F ratio) noexcept
{
    return detail::asin(ratio);
}





namespace detail
{

inline float atan(float ratio) noexcept
{
#if XMIPP4_HAS_BUILTIN(atanf)
    return XMIPP4_BUILTIN(atanf)(ratio);
#else
    return atanf(ratio);
#endif
}

inline double atan(double ratio) noexcept
{
#if XMIPP4_HAS_BUILTIN(atan)
    return XMIPP4_BUILTIN(atan)(ratio);
#else
    return ::atan(ratio);
#endif
}

inline long double atan(long double ratio) noexcept
{
#if XMIPP4_HAS_BUILTIN(atanl)
    return XMIPP4_BUILTIN(atanl)(ratio);
#else
    return atanl(ratio);
#endif
}

} // namespace detail

template <typename F>
inline
typename std::enable_if<std::is_floating_point<F>::value, F>::type
atan(F ratio) noexcept
{
    return detail::atan(ratio);
}





namespace detail
{

inline float atan2(float y, float x) noexcept
{
#if XMIPP4_HAS_BUILTIN(atan2f)
    return XMIPP4_BUILTIN(atan2f)(y, x);
#else
    return atan2f(y, x);
#endif
}

inline double atan2(double y, double x) noexcept
{
#if XMIPP4_HAS_BUILTIN(atan2)
    return XMIPP4_BUILTIN(atan2)(y, x);
#else
    return ::atan2(y, x);
#endif
}

inline long double atan2(long double y, long double x) noexcept
{
#if XMIPP4_HAS_BUILTIN(atan2l)
    return XMIPP4_BUILTIN(atan2l)(y, x);
#else
    return atan2l(y, x);
#endif
}

} // namespace detail

template <typename F>
inline
typename std::enable_if<std::is_floating_point<F>::value, F>::type
atan2(F y, F x) noexcept
{
    return detail::atan2(y, x);
}





template <typename I>
XMIPP4_INLINE_CONSTEXPR
typename std::enable_if<std::is_integral<I>::value, int>::type
cos_pi(I x) noexcept
{
    return (x%2) ? -1 : +1;
}

} // namespace math
} // namespace xmipp4
