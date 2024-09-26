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
    return try_acquire_implementation(INFINITE);
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
