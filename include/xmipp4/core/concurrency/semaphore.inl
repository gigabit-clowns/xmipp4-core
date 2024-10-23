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
