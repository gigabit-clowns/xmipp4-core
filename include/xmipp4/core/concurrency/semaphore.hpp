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
    #include "detail/posix_semaphore.hpp"
    #include "detail/mutex_semaphore.hpp"
    #if defined(XMIPP4_POSIX_SEMAPHORE_IMPLEMENTATION)
        #define XMIPP4_SEMAPHORE_IMPLEMENTATION(n) detail::posix_semaphore
        #define XMIPP4_SEMAPHORE_IMPLEMENTATION_MAX_VALUE \
            XMIPP4_POSIX_SEMAPHORE_IMPLEMENTATION_MAX_VALUE
    #elif defined(XMIPP4_MUTEX_SEMAPHORE_IMPLEMENTATION)
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

/**
 * @brief A thread synchronization semaphore primitive.
 * 
 * The semaphore holds a count. Each arriving thread may call on acquire,
 * which tries to decrement its count. If zero, the thread will be blocked
 * until another thread releases it. Leaving threads may call on release,
 * which increments the counter.
 * 
 * Example use case:
 * Only two threads of a pool of N should be doing I/O intensive tasks. Then,
 * I/O could be protected by a semaphore with an initial count of two, such that
 * each thread before issuing I/O calls tries to acquire the semaphore. When
 * finishing, it should release the semaphore.
 * 
 * @tparam N Maximum value for the counter. By default it is the 
 * implementation's maximum.
 */
template <std::size_t N = XMIPP4_SEMAPHORE_IMPLEMENTATION_MAX_VALUE>
class semaphore
{
public:
    /**
     * @brief Alias for the underlying implementation.
     * 
     */
    using implementation_type = XMIPP4_SEMAPHORE_IMPLEMENTATION(N);

    /**
     * @brief Construct a new semaphore object with an initial count
     * 
     * @param count The initial count.
     * 
     */
    semaphore(std::size_t count);
    semaphore(const semaphore &other) = delete;
    semaphore(semaphore &&other) = delete;
    ~semaphore() = default;

    semaphore& operator=(const semaphore &other) = delete;
    semaphore& operator=(semaphore &&other) = delete;

    /**
     * @brief Acquire a semaphore.
     * 
     * If the current count is zero, the current thread is blocked until
     * another thread releases the semaphore. If multiple threads are waiting
     * at the same time, the unblocked thread is undefined (No priority).
     * When acquiring the semaphore the count is decremented by one.
     * 
     */
    void acquire();

    /**
     * @brief Try to acquire the semaphore without waiting.
     * 
     * If the current count is zero, the semaphore cannot be acquired and 
     * immediately returns false. Otherwise the semaphore is acquired, 
     * decrementing the count by one and true is returned.
     * 
     * @return true The semaphore was acquired
     * @return false The semaphore was not acquired.
     */
    bool try_acquire() noexcept;

    /**
     * @brief Try to acquire the semaphore with a relative timeout.
     *
     * If the current count is zero, the current thread will be blocked for 
     * a maximum time specified by the relative timeout. If another thread 
     * releases it in this period, the semaphore is acquired, 
     * decrementing the count by one and true is returned.

     * @tparam Rep Integral representation for the timeout.
     * @tparam Period Units multiplier for the timeout.
     * @param time Relative timeout.
     * @return true The semaphore was acquired.
     * @return false The wait timed out and the semaphore was not acquired.
     */
    template <typename Rep, typename Period>
    bool try_acquire_for(const std::chrono::duration<Rep, Period> &time);

    /**
     * @brief Try to acquire the semaphore with a absolute timeout.
     *
     * If the current count is zero, the current thread will be blocked for 
     * a maximum time point specified by the absolute timeout. If another thread 
     * releases it in this period, the semaphore is acquired, decrementing the 
     * count by one and true is returned.

     * @tparam Clock clock used for reference for timeout. Must satisfy
     * std::is_clock::value
     * @tparam Duration Specialization of std::chrono::duration
     * @param time Absolute timeout
     * @return true The semaphore was acquired.
     * @return false The wait timed out and the semaphore was not acquired.
     */
    template <typename Clock, typename Duration>
    bool try_acquire_until(const std::chrono::time_point<Clock, Duration>& time);

    /**
     * @brief Release the semaphore. 
     * 
     * This will enable one of the waiting threads to continue. 
     * The current count + n cannot surpass the maximum allowed value for the
     * semaphore, this is the return value of max(). 
     * 
     * @param n How many threads to release. By default only one is allowed.
     * Must be greater than zero and within tolerance to not surpass the 
     * allowed maximum value for the semaphore.
     */
    void release(std::size_t n = 1);

    /**
     * @brief Get the maximum count value for the semaphore class.
     * 
     * @return std::size_t The maximum count value.
     */
    static XMIPP4_CONSTEXPR std::size_t max() noexcept;

private:
    implementation_type m_impl;

};

/**
 * @brief Specialization of semaphore to only allow its count to be 0 or 1.
 * 
 */
using binary_semaphore = semaphore<1>;

/**
 * @brief Specialization of semaphore to allow the implementation defined 
 * maximum value for its count.
 * 
 */
using counting_semaphore = semaphore<>;

} // namespace concurrency
} // namespace xmipp4

#include "semaphore.inl"
