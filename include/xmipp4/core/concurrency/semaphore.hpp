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
    #include <semaphore>
    #define XMIPP4_SEMAPHORE_IMPLEMENTATION(n) std::semaphore<n>
    #define XMIPP4_SEMAPHORE_IMPLEMENTATION_MAX_VALUE (std::semaphore<>::max())
#else 
    #include <limits>
    #include "../platform/operating_system.h"
    #if XMIPP4_POSIX
        #include "detail/posix_semaphore.hpp"
        #define XMIPP4_SEMAPHORE_IMPLEMENTATION(n) detail::posix_semaphore
        #define XMIPP4_SEMAPHORE_IMPLEMENTATION_MAX_VALUE \
            (std::numeric_limits<int>::max())
    #else
        #include "detail/mutex_semaphore.hpp"
        #define XMIPP4_SEMAPHORE_IMPLEMENTATION(n) detail::mutex_semaphore
        #define XMIPP4_SEMAPHORE_IMPLEMENTATION_MAX_VALUE \
            (std::numeric_limits<std::size_t>::max())
    #endif
#endif



#include <chrono>

namespace xmipp4 
{
namespace concurrency
{

template <std::ptrdiff_t N = XMIPP4_SEMAPHORE_IMPLEMENTATION_MAX_VALUE>
class semaphore
{
public:
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

private:
    XMIPP4_SEMAPHORE_IMPLEMENTATION(N) m_impl;
};

using binary_semaphore = semaphore<1>;
using counting_semaphore = semaphore<>;

} // namespace concurrency
} // namespace xmipp4

#include "semaphore.inl"
