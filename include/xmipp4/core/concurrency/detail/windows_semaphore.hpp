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
