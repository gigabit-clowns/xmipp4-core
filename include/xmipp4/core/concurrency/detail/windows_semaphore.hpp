// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "../../platform/operating_system.h"
#if XMIPP4_WINDOWS

#define XMIPP4_WINDOWS_SEMAPHORE_IMPLEMENTATION
#define XMIPP4_WINDOWS_SEMAPHORE_IMPLEMENTATION_MAX_VALUE \
    (std::numeric_limits<int>::max())

extern "C"
{
    #include <windows.h>
}

#include <chrono>
#include <limits>

namespace xmipp4 
{
namespace concurrency
{
namespace detail
{

template <std::size_t N>
class windows_semaphore
{
public:
    windows_semaphore(std::size_t count);
    windows_semaphore(const windows_semaphore &other) = delete;
    windows_semaphore(windows_semaphore &&other) = delete;
    ~windows_semaphore();

    windows_semaphore& operator=(const windows_semaphore &other) = delete;
    windows_semaphore& operator=(windows_semaphore &&other) = delete;

    void acquire();
    bool try_acquire() noexcept;
    template <typename Rep, typename Period>
    bool try_acquire_for(const std::chrono::duration<Rep, Period> &time);
    template <typename Clock, typename Duration>
    bool try_acquire_until(const std::chrono::time_point<Clock, Duration>& time);
    void release(std::size_t n = 1);

private:
    HANDLE m_handle;

    bool try_acquire_implementation(long timeout_ms);

};

} // namespace detail
} // namespace concurrency
} // namespace xmipp4

#include "windows_semaphore.inl"

#endif // XMIPP4_WINDOWS
