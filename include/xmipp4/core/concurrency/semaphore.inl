// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "semaphore.hpp"

namespace xmipp4 
{
namespace concurrency
{

template <std::size_t N>
inline
semaphore<N>::semaphore(std::size_t count)
    : m_impl(count)
{
}

template <std::size_t N>
inline
void semaphore<N>::acquire()
{
    m_impl.acquire();
}

template <std::size_t N>
inline
bool semaphore<N>::try_acquire() noexcept
{
    return m_impl.try_acquire();
}

template <std::size_t N>
template <typename Rep, typename Period>
inline
bool semaphore<N>::try_acquire_for(const std::chrono::duration<Rep, Period> &time)
{
    return m_impl.try_acquire_for(time);
}

template <std::size_t N>
template <typename Clock, typename Duration>
inline
bool semaphore<N>::try_acquire_until(const std::chrono::time_point<Clock, Duration>& time)
{
    return m_impl.try_acquire_until(time);
}

template <std::size_t N>
inline
void semaphore<N>::release(std::size_t n)
{
    m_impl.release(n);
}



template <std::size_t N>
XMIPP4_INLINE_CONSTEXPR
std::size_t semaphore<N>::max() noexcept
{
    return N;
}

} // namespace concurrency
} // namespace xmipp4
