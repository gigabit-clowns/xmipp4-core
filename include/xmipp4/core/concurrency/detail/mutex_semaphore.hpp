// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#define XMIPP4_MUTEX_SEMAPHORE_IMPLEMENTATION
#define XMIPP4_MUTEX_SEMAPHORE_IMPLEMENTATION_MAX_VALUE \
    (std::numeric_limits<std::size_t>::max())

#include <mutex>
#include <condition_variable>
#include <chrono>
#include <limits>

namespace xmipp4 
{
namespace concurrency
{
namespace detail
{

class mutex_semaphore
{
public:
    mutex_semaphore(std::size_t count);
    mutex_semaphore(const mutex_semaphore &other) = delete;
    mutex_semaphore(mutex_semaphore &&other) = delete;
    ~mutex_semaphore() = default;

    mutex_semaphore& operator=(const mutex_semaphore &other) = delete;
    mutex_semaphore& operator=(mutex_semaphore &&other) = delete;

    void acquire();
    bool try_acquire() noexcept;
    template <typename Rep, typename Period>
    bool try_acquire_for(const std::chrono::duration<Rep, Period> &time);
    template <typename Clock, typename Duration>
    bool try_acquire_until(const std::chrono::time_point<Clock, Duration>& time);
    void release(std::size_t n = 1);

private:
    std::mutex m_mutex;
    std::condition_variable m_condition_variable;
    std::size_t m_count;

    bool try_acquire_implementation() noexcept;

};

} // namespace detail
} // namespace concurrency
} // namespace xmipp4

#include "mutex_semaphore.inl"
