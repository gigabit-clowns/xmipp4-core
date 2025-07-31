// SPDX-License-Identifier: GPL-3.0-only

#include "bspline.hpp"

#include "abs.hpp"

namespace xmipp4
{
namespace math
{

template <typename F>
inline
F bspline0(F x) noexcept
{
    const auto ax = std::abs(x);
    return ax < F(0.5) ? F(1.0) : F(0.0);
}

template <typename F>
inline
F bspline1(F x) noexcept
{
    const auto ax = std::abs(x);
    return ax < F(1.0) ? F(1.0) - ax : F(0.0);
}

template <typename F>
inline
F bspline2(F x) noexcept
{
    F result = 0.0;

    const auto ax = abs(x);
    if (ax < F(0.5))
    {
        result = F(0.75) - ax*ax;
    }
    else if (ax < F(1.5))
    {
        result = 0.5*ax*ax - F(1.5)*ax + F(9.0/8.0);
    }

    return result;
}

template <typename F>
inline
F bspline3(F x) noexcept
{
    F result = 0.0;

    const auto ax = abs(x);
    if(ax < F(1))
    {
        result = F(2.0/3.0) + F(0.5)*ax*ax*(ax - F(2.0));
    }
    else if(ax < F(2))
    {
        const auto y = F(2.0) - ax;
        result = F(1.0/6.0)*y*y*y;
    }

    return result;
}

} // namespace math
} // namespace xmipp4
