// SPDX-License-Identifier: GPL-3.0-only

#include "mutex_semaphore.hpp"

namespace xmipp4 
{
namespace concurrency
{
namespace detail
{

inline mutex_semaphore::mutex_semaphore(std::size_t count)
    : m_count(count)
{
}



inline void mutex_semaphore::acquire()
{
    std::unique_lock<std::mutex> lock(m_mutex);

    // Wait until free
    while(m_count == 0)
    {
        m_condition_variable.wait(lock);
    }

    // Acquire
    --m_count;
}

inline bool mutex_semaphore::try_acquire() noexcept
{
    std::lock_guard<std::mutex> lock(m_mutex);
    return try_acquire_implementation();
}

template <typename Rep, typename Period>
inline 
bool mutex_semaphore::try_acquire_for(const std::chrono::duration<Rep, Period> &time)
{
    // Fall back into wait_until
    const auto now = std::chrono::steady_clock::now();
    const auto limit = now + time;
    return try_acquire_until(limit);
}

template <typename Clock, typename Duration>
inline 
bool mutex_semaphore::try_acquire_until(const std::chrono::time_point<Clock, Duration>& time)
{

    std::unique_lock<std::mutex> lock(m_mutex);

    // Wait until acquisition or timeout
    while (m_count == 0 && m_condition_variable.wait_until(lock, time) == std::cv_status::no_timeout);

    // Try to acquire
    return try_acquire_implementation();
}

inline void mutex_semaphore::release(std::size_t n)
{

    std::lock_guard<std::mutex> lock(m_mutex);

    // Release
    m_count += n;

    // Signal waiting threads (if any)
    m_condition_variable.notify_all();
}



inline
bool mutex_semaphore::try_acquire_implementation() noexcept
{    
    const auto result = m_count > 0;
    if (result)
    {
        --m_count;
    }

    return result;
}

} // namespace detail
} // namespace concurrency
} // namespace xmipp4
