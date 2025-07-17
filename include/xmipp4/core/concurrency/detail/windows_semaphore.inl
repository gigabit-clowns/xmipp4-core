// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "windows_semaphore.hpp"

#include <stdexcept>

namespace xmipp4 
{
namespace concurrency
{
namespace detail
{

template <std::size_t N>
inline
windows_semaphore<N>::windows_semaphore(std::size_t count)
    : m_handle(CreateSemaphore(NULL, count, N, NULL))
{
    if (m_handle == nullptr)
    {
        throw std::runtime_error("Could not create semaphore");
    }
}

template <std::size_t N>
inline
windows_semaphore<N>::~windows_semaphore()
{
    CloseHandle(m_handle);
}


template <std::size_t N>
inline
void windows_semaphore<N>::acquire()
{
    try_acquire_implementation(INFINITE);
}

template <std::size_t N>
inline
bool windows_semaphore<N>::try_acquire() noexcept
{
    return try_acquire_implementation(0L);
}

template <std::size_t N>
template <typename Rep, typename Period>
inline
bool windows_semaphore<N>::try_acquire_for(const std::chrono::duration<Rep, Period> &time)
{
    bool result;
    if (time.count() > 0)
    {
        const auto ms = 
            std::chrono::duration_cast<std::chrono::milliseconds>(time);
        result = try_acquire_implementation(ms.count());
    }
    else
    {
        result = try_acquire_implementation(0L);
    }
    return result;
}

template <std::size_t N>
template <typename Clock, typename Duration>
inline
bool windows_semaphore<N>::try_acquire_until(const std::chrono::time_point<Clock, Duration> &time)
{
    // Fall back into try_acquire_for
    const auto now = Clock::now();
    const auto delta = time - now;
    return try_acquire_for(delta);
}

template <std::size_t N>
inline
void windows_semaphore<N>::release(std::size_t n)
{
    ReleaseSemaphore(m_handle, n, NULL);
}



template <std::size_t N>
inline
bool windows_semaphore<N>::try_acquire_implementation(long timeout_ms)
{
    const auto err = WaitForSingleObject(m_handle, timeout_ms);

    // Evaluate return code
    bool result;
    switch (err)
    {
    case WAIT_OBJECT_0:
        result = true;
        break;

    case WAIT_TIMEOUT: 
        result = false;
        break;

    default:
        throw std::runtime_error("Error trying to acquire the semaphore");
    }

    return result;
}

} // namespace detail
} // namespace concurrency
} // namespace xmipp4
