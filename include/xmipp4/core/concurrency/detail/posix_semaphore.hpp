// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "../../platform/operating_system.h"
#if XMIPP4_POSIX && (_POSIX_C_SOURCE >= 200112L)

#define XMIPP4_POSIX_SEMAPHORE_IMPLEMENTATION
#define XMIPP4_POSIX_SEMAPHORE_IMPLEMENTATION_MAX_VALUE \
    (std::numeric_limits<int>::max())

extern "C"
{
#include <semaphore.h>
}

#include <chrono>
#include <limits>

namespace xmipp4 
{
namespace concurrency
{
namespace detail
{

class posix_semaphore
{
public:
    posix_semaphore(uint count, bool shared = false);
    posix_semaphore(const posix_semaphore &other) = delete;
    posix_semaphore(posix_semaphore &&other) = delete;
    ~posix_semaphore();

    posix_semaphore& operator=(const posix_semaphore &other) = delete;
    posix_semaphore& operator=(posix_semaphore &&other) = delete;

    void acquire();
    bool try_acquire() noexcept;
    template <typename Rep, typename Period>
    bool try_acquire_for(const std::chrono::duration<Rep, Period> &time);
    template <typename Clock, typename Duration>
    bool try_acquire_until(const std::chrono::time_point<Clock, Duration>& time);
    void release(std::size_t n = 1);

private:
    sem_t m_impl;

};

} // namespace detail
} // namespace concurrency
} // namespace xmipp4

#include "posix_semaphore.inl"

#endif // XMIPP4_POSIX
