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
 * @file bessel.inl
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Implementation of bessel.hpp
 * @date 2024-04-15
 * 
 */

#include "bessel.hpp"

#include "abs.hpp"
#include "arithmetic.hpp"
#include "power.hpp"
#include "trigonometric.hpp"
#include "exponential.hpp"
#include "factorial.hpp"
#include "../platform/builtin.h"

// Check MSVC-specific support for bessel functions
#if defined(_MSC_VER)
#define XMIPP4_MSVC_BESSEL
    #if _MSC_VER >= 1800 // MSVC 2013 and later
        #define XMIPP4_MSVC_NATIVE_BESSEL
    #endif
#endif

#include <limits>
#include <cmath>

namespace xmipp4
{
namespace math
{

namespace detail
{

template <typename F>
inline
typename std::enable_if<std::is_floating_point<F>::value, F>::type
cylindrical_bessel_j0(F x) noexcept
{
    // Based on:
    // https://www.atnf.csiro.au/computing/software/gipsy/sub/bessel.c
    F ax,z;
    F xx,y,ans,ans1,ans2;

    if ((ax=abs(x)) < F(8)) 
    {
        y=x*x;
        ans1=F(57568490574.0)+y*(F(-13362590354.0)+y*(F(651619640.7)
          +y*(F(-11214424.18)+y*(F(77392.33017)+y*(F(-184.9052456))))));
        ans2=F(57568490411.0)+y*(F(1029532985.0)+y*(F(9494680.718)
          +y*(F(59272.64853)+y*(F(267.8532712)+y*F(1)))));

        ans=ans1/ans2;
    } 
    else 
    {
        z=F(8.0)/ax;
        y=z*z;
        xx=ax-F(0.785398164);
        ans1=F(1)+y*(F(-0.1098628627e-2)+y*(F(0.2734510407e-4)
          +y*(F(-0.2073370639e-5)+y*F(0.2093887211e-6))));
        ans2 = F(-0.1562499995e-1)+y*(F(0.1430488765e-3)
          +y*(F(-0.6911147651e-5)+y*(F(0.7621095161e-6)
          -y*F(0.934935152e-7))));

        ans=sqrt(F(0.636619772)/ax)*(cos(xx)*ans1-z*sin(xx)*ans2);
    }

    return ans;
}

#if XMIPP4_HAS_BUILTIN(j0f)
inline float cylindrical_bessel_j0(float x) noexcept
{
    return XMIPP4_BUILTIN(j0f)(x);
}
#endif

#if XMIPP4_HAS_BUILTIN(j0)
inline double cylindrical_bessel_j0(double x) noexcept
{
    return XMIPP4_BUILTIN(j0)(x);
}
#elif defined(XMIPP4_MSVC_NATIVE_BESSEL) 
inline double cylindrical_bessel_j0(double x) noexcept
{
    return _j0(x);
}
#endif

#if XMIPP4_HAS_BUILTIN(j0l)
inline long double cylindrical_bessel_j0(long double x) noexcept
{
    return XMIPP4_BUILTIN(j0l)(x);
}
#endif

} // namespace detail

template <typename F>
inline
typename std::enable_if<std::is_floating_point<F>::value, F>::type
cylindrical_bessel_j0(F x) noexcept
{
    return detail::cylindrical_bessel_j0(x);
}





namespace detail
{

template <typename F>
inline
typename std::enable_if<std::is_floating_point<F>::value, F>::type
cylindrical_bessel_j1(F x) noexcept
{
    // Based on:
    // https://www.atnf.csiro.au/computing/software/gipsy/sub/bessel.c
    F ax,z;
    F xx,y,ans,ans1,ans2;

    if ((ax=abs(x)) < F(8)) 
    {
        y=x*x;
        ans1=x*(F(72362614232.0)+y*(F(-7895059235.0)+y*(F(242396853.1)
         +y*(F(-2972611.439)+y*(F(15704.48260)+y*(F(-30.16036606)))))));
        ans2=F(144725228442.0)+y*(F(2300535178.0)+y*(F(18583304.74)
         +y*(F(99447.43394)+y*(F(376.9991397)+y*F(1)))));

        ans=ans1/ans2;
    }
    else
    {
        z=8.0/ax;
        y=z*z;
        xx=ax-2.356194491;
        ans1=F(1)+y*(F(0.183105e-2)+y*(F(-0.3516396496e-4)
         +y*(F(0.2457520174e-5)+y*(F(-0.240337019e-6)))));
        ans2=F(0.04687499995)+y*(F(-0.2002690873e-3)
         +y*(F(0.8449199096e-5)+y*(F(-0.88228987e-6)
         +y*F(0.105787412e-6))));

        ans=sqrt(F(0.636619772)/ax)*(cos(xx)*ans1-z*sin(xx)*ans2);
        if (x < 0) ans = -ans;
    }

    return ans;
}

#if XMIPP4_HAS_BUILTIN(j1f)
inline float cylindrical_bessel_j1(float x) noexcept
{
    return XMIPP4_BUILTIN(j1f)(x);
}
#endif

#if XMIPP4_HAS_BUILTIN(j1)
inline double cylindrical_bessel_j1(double x) noexcept
{
    return XMIPP4_BUILTIN(j1)(x);
}
#elif defined(XMIPP4_MSVC_NATIVE_BESSEL) 
inline double cylindrical_bessel_j1(double x) noexcept
{
    return _j1(x);
}
#endif

#if XMIPP4_HAS_BUILTIN(j1l)
inline long double cylindrical_bessel_j1(long double x) noexcept
{
    return XMIPP4_BUILTIN(j1l)(x);
}
#endif

} // namespace detail

template <typename F>
inline
typename std::enable_if<std::is_floating_point<F>::value, F>::type
cylindrical_bessel_j1(F x) noexcept
{
    return detail::cylindrical_bessel_j1(x);
}





namespace detail
{

template <typename F>
inline
typename std::enable_if<std::is_floating_point<F>::value, F>::type
cylindrical_bessel_jn_iterative(int n, F x, F eps = F(1e-12)) noexcept
{
    int sign = 1;
    if (n < 0)
    {
        n = -n;
        sign = cos_pi(n);
    }

    F result;
    if (abs(x) > n)
    {
        // Based on:
        // https://www.atnf.csiro.au/computing/software/gipsy/sub/bessel.c
        const F tox = F(2) / x;
        F bjm = cylindrical_bessel_j0(x);
        F bj = cylindrical_bessel_j1(x);
        for(unsigned k = 1; k < static_cast<unsigned>(n); ++k)
        {
            const auto temp = k*tox*bj-bjm;
            bjm=bj;
            bj=temp;
        }

        result = bj;
    }
    else
    {
        // Based on:
        // https://www.bragitoff.com/2017/08/bessel-function-series-c-program/
        F term = F(1) / large_factorial<F>(static_cast<unsigned>(n));
        F sum = term;
        std::size_t k = 1;
        do
        {
            const auto term_ratio = -(x*x) / static_cast<F>(4*k*(n+k));
            term *= term_ratio;
            sum += term;
            ++k;
        }
        while (abs(term/sum) > eps);

        result = sum * pow(x/F(2), static_cast<F>(n));
    }

    result *= sign;
    return result;
}

template <typename F>
inline
typename std::enable_if<std::is_floating_point<F>::value, F>::type
cylindrical_bessel_jn(int n, F x) noexcept
{
    // Based on:
    // https://www.atnf.csiro.au/computing/software/gipsy/sub/bessel.c
    switch (n)
    {
    case 0: return cylindrical_bessel_j0(x);
    case 1: return cylindrical_bessel_j1(x);
    case -1: return -cylindrical_bessel_j1(x);
    default: return cylindrical_bessel_jn_iterative(n, x);
    }
}

#if XMIPP4_HAS_BUILTIN(jnf)
inline float cylindrical_bessel_jn(int n, float x) noexcept
{
    return XMIPP4_BUILTIN(jnf)(n, x);
}
#endif

#if XMIPP4_HAS_BUILTIN(jn)
inline double cylindrical_bessel_jn(int n, double x) noexcept
{
    return XMIPP4_BUILTIN(jn)(n, x);
}
#elif defined(XMIPP4_MSVC_NATIVE_BESSEL) 
inline double cylindrical_bessel_jn(int n, double x) noexcept
{
    return _jn(n, x);
}
#endif

#if XMIPP4_HAS_BUILTIN(jnl)
inline long double cylindrical_bessel_jn(int n, long double x) noexcept
{
    return XMIPP4_BUILTIN(jnl)(n, x);
}
#endif

} // namespace detail

template <typename F>
inline
typename std::enable_if<std::is_floating_point<F>::value, F>::type
cylindrical_bessel_jn(int n, F x) noexcept
{
    return detail::cylindrical_bessel_jn(n, x);
}





namespace detail
{


template <typename F>
inline
typename std::enable_if<std::is_floating_point<F>::value, F>::type
cylindrical_bessel_y0(F x) noexcept
{
    // Based on:
    // https://www.atnf.csiro.au/computing/software/gipsy/sub/bessel.c
    F z;
    F xx,y,ans,ans1,ans2;

    if(x < 0)
    {
        ans = std::nan("");
    }
    else if (x == 0)
    {
        ans = -std::numeric_limits<F>::infinity();
    }
    else if (x < F(8)) 
    {
        y=x*x;
        ans1 = F(-2957821389.0)+y*(F(7062834065.0)+y*(F(-512359803.6)
         +y*(F(10879881.29)+y*(F(-86327.92757)+y*F(228.4622733)))));
        ans2=F(40076544269.0)+y*(F(745249964.8)+y*(F(7189466.438)
         +y*(F(47447.26470)+y*(F(226.1030244)+y*F(1)))));

        ans=(ans1/ans2)+F(0.636619772)*cylindrical_bessel_j0(x)*log(x);
    } 
    else 
    {
        z=F(8)/x;
        y=z*z;
        xx=x-F(0.785398164);
        ans1=F(1)+y*(F(-0.1098628627e-2)+y*(F(0.2734510407e-4)
           +y*(F(-0.2073370639e-5)+y*F(0.2093887211e-6))));
        ans2 = F(-0.1562499995e-1)+y*(F(0.1430488765e-3)
           +y*(F(-0.6911147651e-5)+y*(F(0.7621095161e-6)
           +y*(F(-0.934945152e-7)))));

        ans=sqrt(F(0.636619772)/x)*(sin(xx)*ans1+z*cos(xx)*ans2);
    }

    return ans;
}

#if XMIPP4_HAS_BUILTIN(y0f)
inline float cylindrical_bessel_y0(float x) noexcept
{
    return XMIPP4_BUILTIN(y0f)(x);
}
#endif

#if XMIPP4_HAS_BUILTIN(y0)
inline double cylindrical_bessel_y0(double x) noexcept
{
    return XMIPP4_BUILTIN(y0)(x);
}
#elif defined(XMIPP4_MSVC_NATIVE_BESSEL) 
inline double cylindrical_bessel_y0(double x) noexcept
{
    return _y0(x);
}
#endif

#if XMIPP4_HAS_BUILTIN(y0l)
inline long double cylindrical_bessel_y0(long double x) noexcept
{
    return XMIPP4_BUILTIN(y0l)(x);
}
#endif

} // namespace detail

template <typename F>
inline
typename std::enable_if<std::is_floating_point<F>::value, F>::type
cylindrical_bessel_y0(F x) noexcept
{
    return detail::cylindrical_bessel_y0(x);
}





namespace detail
{

template <typename F>
inline
typename std::enable_if<std::is_floating_point<F>::value, F>::type
cylindrical_bessel_y1(F x) noexcept
{
    // Based on:
    // https://www.atnf.csiro.au/computing/software/gipsy/sub/bessel.c
    F z;
    F xx,y,ans,ans1,ans2;

    if(x < 0)
    {
        ans = std::nan("");
    }
    else if (x == 0)
    {
        ans = -std::numeric_limits<F>::infinity();
    }
    else if (x < F(8)) 
    {
      y=x*x;
      ans1=x*(F(-0.4900604943e13)+y*(F(0.1275274390e13)
         +y*(F(-0.5153438139e11)+y*(F(0.7349264551e9)
         +y*(F(-0.4237922726e7)+y*F(0.8511937935e4))))));
      ans2=F(0.2499580570e14)+y*(F(0.4244419664e12)
         +y*(F(0.3733650367e10)+y*(F(0.2245904002e8)
         +y*(F(0.1020426050e6)+y*(F(0.3549632885e3)+y)))));

      ans=(ans1/ans2)+F(0.636619772)*(cylindrical_bessel_j1(x)*log(x)-F(1)/x);
    } 
    else
    {
      z=F(8)/x;
      y=z*z;
      xx=x-F(2.356194491);
      ans1=F(1)+y*(F(0.183105e-2)+y*(F(-0.3516396496e-4)
         +y*(F(0.2457520174e-5)+y*(F(-0.240337019e-6)))));
      ans2=F(0.04687499995)+y*(F(-0.2002690873e-3)
         +y*(F(0.8449199096e-5)+y*(F(-0.88228987e-6)
         +y*F(0.105787412e-6))));

      ans=sqrt(F(0.636619772)/x)*(sin(xx)*ans1+z*cos(xx)*ans2);
    }

    return ans;
}

#if XMIPP4_HAS_BUILTIN(y1f)
inline float cylindrical_bessel_y1(float x) noexcept
{
    return XMIPP4_BUILTIN(y1f)(x);
}
#endif

#if XMIPP4_HAS_BUILTIN(y1)
inline double cylindrical_bessel_y1(double x) noexcept
{
    return XMIPP4_BUILTIN(y1)(x);
}
#elif defined(XMIPP4_MSVC_NATIVE_BESSEL) 
inline double cylindrical_bessel_y1(double x) noexcept
{
    return _y1(x);
}
#endif

#if XMIPP4_HAS_BUILTIN(y1l)
inline long double cylindrical_bessel_y1(long double x) noexcept
{
    return XMIPP4_BUILTIN(y1l)(x);
}
#endif

} // namespace detail

template <typename F>
inline
typename std::enable_if<std::is_floating_point<F>::value, F>::type
cylindrical_bessel_y1(F x) noexcept
{
    return detail::cylindrical_bessel_y1(x);
}





namespace detail
{

template <typename F>
inline
typename std::enable_if<std::is_floating_point<F>::value, F>::type
cylindrical_bessel_yn_iterative(int n, F x) noexcept
{
    int sign = 1;
    if (n < 0)
    {
        n = -n;
        sign = cos_pi(n);
    }

    F result;
    if(x < 0)
    {
        result = std::nan("");
    }
    else if (x == 0)
    {
        result = -std::numeric_limits<F>::infinity();
    }
    else
    {
        // Based on:
        // https://www.atnf.csiro.au/computing/software/gipsy/sub/bessel.c
        F tox = F(2) / x;
        F bym = cylindrical_bessel_y0(x);
        F by = cylindrical_bessel_y1(x);
        for(unsigned k = 1; k < static_cast<unsigned>(n); ++k)
        {
            const auto temp = k*tox*by-bym;
            bym=by;
            by=temp;
        }

        result = by;
    }

    return sign*result;
}

template <typename F>
inline
typename std::enable_if<std::is_floating_point<F>::value, F>::type
cylindrical_bessel_yn(int n, F x) noexcept
{
    // Based on:
    // https://www.atnf.csiro.au/computing/software/gipsy/sub/bessel.c
    switch (n)
    {
    case 0: return cylindrical_bessel_y0(x);
    case 1: return cylindrical_bessel_y1(x);
    case -1: return -cylindrical_bessel_y1(x);
    default: return cylindrical_bessel_yn_iterative(n, x);
    }
}

#if XMIPP4_HAS_BUILTIN(ynf)
inline float cylindrical_bessel_yn(int n, float x) noexcept
{
    return XMIPP4_BUILTIN(ynf)(n, x);
}
#endif

#if XMIPP4_HAS_BUILTIN(yn)
inline double cylindrical_bessel_yn(int n, double x) noexcept
{
    return XMIPP4_BUILTIN(yn)(n, x);
}
#elif defined(XMIPP4_MSVC_NATIVE_BESSEL) 
inline double cylindrical_bessel_yn(int n, double x) noexcept
{
    return _yn(n, x);
}
#endif

#if XMIPP4_HAS_BUILTIN(ynl)
inline long double cylindrical_bessel_yn(int n, long double x) noexcept
{
    return XMIPP4_BUILTIN(ynl)(n, x);
}
#endif

} // namespace detail

template <typename F>
inline
typename std::enable_if<std::is_floating_point<F>::value, F>::type
cylindrical_bessel_yn(int n, F x) noexcept
{
    return detail::cylindrical_bessel_yn(n, x);
}





namespace detail
{

template <typename F>
inline
typename std::enable_if<std::is_floating_point<F>::value, F>::type
cylindrical_bessel_i0(F x) noexcept
{
    // Based on:
    // https://www.atnf.csiro.au/computing/software/gipsy/sub/bessel.c
    F y, ax, ans;

    ax = math::abs(x);
    if (ax < F(3.75))
    {
        y = x / F(3.75);
        y *= y;
        ans = F(1)+y*(F(3.5156229)+y*(F(3.0899424)+y*(F(1.2067492)
                +y*(F(0.2659732)+y*(F(0.360768e-1)+y*F(0.45813e-2))))));
    }
    else
    {
        y = F(3.75) / ax;
        ans = (exp(ax) / sqrt(ax)) * (F(0.39894228)+y*(F(0.1328592e-1)
                +y*(F(0.225319e-2)+y*(F(-0.157565e-2)+y*(F(0.916281e-2)
                +y*(F(-0.2057706e-1)+y*(F(0.2635537e-1)+y*(F(-0.1647633e-1)
                +y*F(0.392377e-2)))))))));
    }

    return ans;
}

} // namespace detail

template <typename F>
inline
typename std::enable_if<std::is_floating_point<F>::value, F>::type
cylindrical_bessel_i0(F x) noexcept
{
    return detail::cylindrical_bessel_i0(x);
}





namespace detail
{

template <typename F>
inline
typename std::enable_if<std::is_floating_point<F>::value, F>::type
cylindrical_bessel_i1(F x) noexcept
{
    // Based on:
    // https://www.atnf.csiro.au/computing/software/gipsy/sub/bessel.c
    F y, ax, ans;
    
    ax = math::abs(x);
    if (ax < F(3.75))
    {
        y = x / F(3.75);
        y *= y;
        ans = ax * (0.5+y*(0.87890594+y*(0.51498869+y*(0.15084934
                    +y*(0.2658733e-1+y*(0.301532e-2+y* 0.32411e-3))))));
    }
    else
    {
        y = F(3.75) / ax;
        ans = F(0.2282967e-1)+y*(F(-0.2895312e-1)+y*(F(0.1787654e-1)
                -y* F(0.420059e-2)));
        ans = F(0.39894228)+y*(F(-0.3988024e-1)+y*(F(-0.362018e-2)
                +y*(F(0.163801e-2)+y*(F(-0.1031555e-1)+y*ans))));
        ans *= (exp(ax) / sqrt(ax));
    }
    return x < 0 ? -ans : ans;
}

} // namespace detail

template <typename F>
inline
typename std::enable_if<std::is_floating_point<F>::value, F>::type
cylindrical_bessel_i1(F x) noexcept
{
    return detail::cylindrical_bessel_i1(x);
}





namespace detail
{

template <typename F>
inline
typename std::enable_if<std::is_floating_point<F>::value, F>::type
cylindrical_bessel_in_iterative(int n, F x) noexcept
{
    F result;

    if (x == 0)
    {
        result = 0.0;
    }
    else
    {
        n = math::abs(n);

        // Based on:
        // https://www.atnf.csiro.au/computing/software/gipsy/sub/bessel.c
        const F tox = F(2) / abs(x);
        F bip = F(0);
        F bi = F(1);
        F ans;
        const unsigned m = 2*(n + static_cast<unsigned>(sqrt(F(40.0)*n)));
        for(unsigned k=m; k>0; k--)
        {
            const auto tmp = bip+k*tox*bi;
            bip=bi;
            bi=tmp;

            // Normalize to avoid stability issues
            const auto abi = abs(bi);
            ans /= abi;
            bip /= abi;
            bi = sign(bi);
            if(k == n) ans = bip;
        }
        ans *= cylindrical_bessel_i0(x) / bi;
        result = x < 0.0 && n%2 == 1 ? -ans : ans;
    }

    return result;
}

template <typename F>
inline
typename std::enable_if<std::is_floating_point<F>::value, F>::type
cylindrical_bessel_in(int n, F x) noexcept
{
    switch (n)
    {
    case 0: return cylindrical_bessel_i0(x);
    case 1: case -1: return cylindrical_bessel_i1(x);
    default: return cylindrical_bessel_in_iterative(n, x);
    }
}

} // namespace detail

template <typename F>
inline
typename std::enable_if<std::is_floating_point<F>::value, F>::type
cylindrical_bessel_in(int n, F x) noexcept
{
    return detail::cylindrical_bessel_in(n, x);
}





namespace detail
{

template <typename F>
inline
typename std::enable_if<std::is_floating_point<F>::value, F>::type
cylindrical_bessel_k0(F x) noexcept
{
    // Based on:
    // https://www.atnf.csiro.au/computing/software/gipsy/sub/bessel.c
    F y, ans;

    if(x < 0)
    {
        ans = std::nan("");
    }
    else if (x == 0)
    {
        ans = std::numeric_limits<F>::infinity();
    }
    else if (x <= F(2)) 
    {
        y=x*x/F(4);
        ans=(-log(x/F(2))*cylindrical_bessel_i0(x))+(F(-0.57721566)+y*(F(0.42278420)
            +y*(F(0.23069756)+y*(F(0.3488590e-1)+y*(F(0.262698e-2)
            +y*(F(0.10750e-3)+y*F(0.74e-5)))))));
    } 
    else 
    {
        y=F(2)/x;
        ans=(exp(-x)/sqrt(x))*(F(1.25331414)+y*(F(-0.7832358e-1)
            +y*(F(0.2189568e-1)+y*(F(-0.1062446e-1)+y*(F(0.587872e-2)
            +y*(F(-0.251540e-2)+y*F(0.53208e-3)))))));
    }

    return ans;
}

} // namespace detail

template <typename F>
inline
typename std::enable_if<std::is_floating_point<F>::value, F>::type
cylindrical_bessel_k0(F x) noexcept
{
    return detail::cylindrical_bessel_k0(x);
}





namespace detail
{

template <typename F>
inline
typename std::enable_if<std::is_floating_point<F>::value, F>::type
cylindrical_bessel_k1(F x) noexcept
{
    // Based on:
    // https://www.atnf.csiro.au/computing/software/gipsy/sub/bessel.c
    F y, ans;

    if(x < 0)
    {
        ans = std::nan("");
    }
    else if (x == 0)
    {
        ans = std::numeric_limits<F>::infinity();
    }
    else if (x <= F(2)) 
    {
        y=x*x/F(4);
        ans=(log(x/F(2))*cylindrical_bessel_i1(x))+(F(1)/x)*(F(1)+y*(F(0.15443144)
            +y*(F(-0.67278579)+y*(F(-0.18156897)+y*(F(-0.1919402e-1)
            +y*(F(-0.110404e-2)+y*(F(-0.4686e-4))))))));
    } 
    else 
    {
        y=F(2)/x;
        ans=(exp(-x)/sqrt(x))*(F(1.25331414)+y*(F(0.23498619)
            +y*(F(-0.3655620e-1)+y*(F(0.1504268e-1)+y*(F(-0.780353e-2)
            +y*(F(0.325614e-2)+y*(F(-0.68245e-3))))))));
    }

    return ans;
}

} // namespace detail

template <typename F>
inline
typename std::enable_if<std::is_floating_point<F>::value, F>::type
cylindrical_bessel_k1(F x) noexcept
{
    return detail::cylindrical_bessel_k1(x);
}





namespace detail
{

template <typename F>
inline
typename std::enable_if<std::is_floating_point<F>::value, F>::type
cylindrical_bessel_kn_iterative(int n, F x) noexcept
{
    F result;

    if (x < 0)
    {
        result = std::nan("");
    }
    else if (x == 0)
    {
        result = std::numeric_limits<F>::infinity();
    }
    else
    {
        n = math::abs(n);

        // Based on:
        // https://www.atnf.csiro.au/computing/software/gipsy/sub/bessel.c
        const F tox = F(2)/x;
        F bkm = cylindrical_bessel_k0(x);
        F bk = cylindrical_bessel_k1(x);
        for (unsigned k=1; k<static_cast<unsigned>(n); k++) {
            const auto temp = bkm+k*tox*bk;
            bkm = bk;
            bk = temp;
        }
        
        result = bk;
    }

    return result;
}

template <typename F>
inline
typename std::enable_if<std::is_floating_point<F>::value, F>::type
cylindrical_bessel_kn(int n, F x) noexcept
{
    switch (n)
    {
    case 0: return cylindrical_bessel_k0(x);
    case 1: case -1: return cylindrical_bessel_k1(x);
    default: return cylindrical_bessel_kn_iterative(n, x);
    }
}

} // namespace detail

template <typename F>
inline
typename std::enable_if<std::is_floating_point<F>::value, F>::type
cylindrical_bessel_kn(int n, F x) noexcept
{
    return detail::cylindrical_bessel_kn(n, x);
}


} // namespace math
} // namespace xmipp4
