#pragma once

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


// Detect underlying implementation
#include "../platform/cpp_features.hpp"
#if XMIPP4_HAS_CPP_FEATURE(__cpp_lib_semaphore, 201907L)
    // Try with C++20 semaphore
    #include <semaphore>
    #define XMIPP4_SEMAPHORE_IMPLEMENTATION(n) std::semaphore<n>
    #define XMIPP4_SEMAPHORE_IMPLEMENTATION_MAX_VALUE (std::semaphore<>::max())
#else 
    // Fall back to one of the manual implementations or wrappers
    #include "detail/windows_semaphore.hpp"
    #include "detail/mutex_semaphore.hpp"
    #if defined(XMIPP4_WINDOWS_SEMAPHORE_IMPLEMENTATION_MAX_VALUE)
        #define XMIPP4_SEMAPHORE_IMPLEMENTATION(n) detail::windows_semaphore<n>
        #define XMIPP4_SEMAPHORE_IMPLEMENTATION_MAX_VALUE \
            XMIPP4_WINDOWS_SEMAPHORE_IMPLEMENTATION_MAX_VALUE
    #elif defined(XMIPP4_MUTEX_SEMAPHORE_IMPLEMENTATION_MAX_VALUE)
        #define XMIPP4_SEMAPHORE_IMPLEMENTATION(n) detail::mutex_semaphore
        #define XMIPP4_SEMAPHORE_IMPLEMENTATION_MAX_VALUE \
            XMIPP4_MUTEX_SEMAPHORE_IMPLEMENTATION_MAX_VALUE
    #else
        #error "No implementation available for semaphore"
    #endif
#endif

#include "../platform/constexpr.hpp"

#include <chrono>

namespace xmipp4 
{
namespace concurrency
{

template <std::size_t N = XMIPP4_SEMAPHORE_IMPLEMENTATION_MAX_VALUE>
class semaphore
{
public:
    using implementation_type = XMIPP4_SEMAPHORE_IMPLEMENTATION(N);

    semaphore(std::size_t count);
    semaphore(const semaphore &other) = delete;
    semaphore(semaphore &&other) = delete;
    ~semaphore() = default;

    semaphore& operator=(const semaphore &other) = delete;
    semaphore& operator=(semaphore &&other) = delete;

    void acquire();
    bool try_acquire() noexcept;
    template <typename Rep, typename Period>
    bool try_acquire_for(const std::chrono::duration<Rep, Period> &time);
    template <typename Clock, typename Duration>
    bool try_acquire_until(const std::chrono::time_point<Clock, Duration>& time);
    void release(std::size_t n = 1);

    static XMIPP4_CONSTEXPR std::size_t max() noexcept;

private:
    implementation_type m_impl;
};

using binary_semaphore = semaphore<1>;
using counting_semaphore = semaphore<>;

} // namespace concurrency
} // namespace xmipp4

#include "semaphore.inl"
