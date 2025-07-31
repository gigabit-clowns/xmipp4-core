// SPDX-License-Identifier: GPL-3.0-only

#include "erf.hpp"

#include "../platform/builtin.h"

#include <cmath>

namespace xmipp4
{
namespace math
{

namespace detail
{

inline float erf(float x) noexcept
{
#if XMIPP4_HAS_BUILTIN(erff)
    return XMIPP4_BUILTIN(erff)(x);
#else
    return erff(x);
#endif
}

inline double erf(double x) noexcept
{
#if XMIPP4_HAS_BUILTIN(erf)
    return XMIPP4_BUILTIN(erf)(x);
#else
    return ::erf(x);
#endif
}

inline long double erf(long double x) noexcept
{
#if XMIPP4_HAS_BUILTIN(erfl)
    return XMIPP4_BUILTIN(erfl)(x);
#else
    return erfl(x);
#endif
}

} // namespace detail

template <typename F>
inline
F erf(F x) noexcept
{
    return detail::erf(x);
}





namespace detail
{

inline float erfc(float x) noexcept
{
#if XMIPP4_HAS_BUILTIN(erfcf)
    return XMIPP4_BUILTIN(erfcf)(x);
#else
    return erfcf(x);
#endif
}

inline double erfc(double x) noexcept
{
#if XMIPP4_HAS_BUILTIN(erfc)
    return XMIPP4_BUILTIN(erfc)(x);
#else
    return ::erfc(x);
#endif
}

inline long double erfc(long double x) noexcept
{
#if XMIPP4_HAS_BUILTIN(erfcl)
    return XMIPP4_BUILTIN(erfcl)(x);
#else
    return erfcl(x);
#endif
}

} // namespace detail

template <typename F>
inline
F erfc(F x) noexcept
{
    return detail::erfc(x);
}

} // namespace math
} // namespace xmipp4
