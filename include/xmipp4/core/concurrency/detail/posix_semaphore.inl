// SPDX-License-Identifier: GPL-3.0-only

#include "posix_semaphore.hpp"

#include <stdexcept>

#include <time.h>

namespace xmipp4 
{
namespace concurrency
{
namespace detail
{

inline posix_semaphore::posix_semaphore(uint count, bool shared)
{
    if(sem_init(&m_impl, shared, count) == -1)
    {
        throw std::runtime_error("Could not create semaphore");
    }
}

inline posix_semaphore::~posix_semaphore()
{
    sem_destroy(&m_impl);
}



inline void posix_semaphore::acquire()
{
    if(sem_wait(&m_impl) == -1)
    {
        throw std::runtime_error("Error acquiring the semaphore");
    }
}

inline bool posix_semaphore::try_acquire() noexcept
{
    return sem_trywait(&m_impl) != -1;
}

template <typename Rep, typename Period>
inline 
bool posix_semaphore::try_acquire_for(const std::chrono::duration<Rep, Period> &time)
{
    bool result;
    if (time.count() > 0)
    {
        // Convert duration to timespec's units (seconds + nanoseconds)
        const auto seconds = 
            std::chrono::duration_cast<std::chrono::seconds>(time);
        const auto nanoseconds = 
            std::chrono::duration_cast<std::chrono::nanoseconds>(time - seconds);

        // Ensemble a timespec with the newly converted values
        timespec delta;
        delta.tv_sec = seconds.count();
        delta.tv_nsec = nanoseconds.count();

        // Get current time
        timespec now;
        clock_gettime(CLOCK_REALTIME, &now);

        // Compute the timeout
        timespec timeout;
        timeout.tv_sec = now.tv_sec + delta.tv_sec;
        timeout.tv_nsec = now.tv_nsec + delta.tv_nsec;
        if(timeout.tv_nsec >= 1000000000) 
        {
            timeout.tv_nsec -= 1000000000;
            ++timeout.tv_sec;
        }

        // Wait to acquire
        result = sem_timedwait(&m_impl, &timeout) == 0;
    }
    else
    {
        result = try_acquire();
    }
    return result;
}

template <typename Clock, typename Duration>
inline
bool posix_semaphore::try_acquire_until(const std::chrono::time_point<Clock, Duration>& time)
{
    // Fallback onto try_wait_for
    const auto now = Clock::now();
    const auto delta = time - now;
    return try_acquire_for(delta);
}

inline void posix_semaphore::release(std::size_t n)
{
    for (std::size_t i = 0; i < n; ++i)
    {
        if(sem_post(&m_impl) == -1)
        {
            throw std::runtime_error("Error releasing the semaphore");
        }
    }
}

} // namespace detail
} // namespace concurrency
} // namespace xmipp4
