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


// Detect underlying implementation
#include "../platform/cpp_features.hpp"
#if XMIPP4_HAS_CPP_FEATURE(__cpp_lib_semaphore, 201907L)
    #include <semaphore>
    #define XMIPP4_COUNTING_SEMAPHORE_IMPLEMENTATION std::counting_semaphore
#else 
    #include "../platform/operating_system.h"
    #if XMIPP4_POSIX
        #include "detail/posix_semaphore.hpp"
        #define XMIPP4_COUNTING_SEMAPHORE_IMPLEMENTATION detail::posix_semaphore
    #else
        #include "detail/mutex_semaphore.hpp"
        #define XMIPP4_COUNTING_SEMAPHORE_IMPLEMENTATION detail::mutex_semaphore
    #endif
#endif



#include <chrono>

namespace xmipp4 
{
namespace concurrency
{

class counting_semaphore
{
public:
    counting_semaphore(std::size_t count);
    counting_semaphore(const counting_semaphore &other) = delete;
    counting_semaphore(counting_semaphore &&other) = delete;
    ~counting_semaphore() = default;

    counting_semaphore& operator=(const counting_semaphore &other) = delete;
    counting_semaphore& operator=(counting_semaphore &&other) = delete;

    void acquire();
    bool try_acquire() noexcept;
    template <typename Rep, typename Period>
    bool try_acquire_for(const std::chrono::duration<Rep, Period> &time);
    template <typename Clock, typename Duration>
    bool try_acquire_until(const std::chrono::time_point<Clock, Duration>& time);
    void release();

private:
    XMIPP4_COUNTING_SEMAPHORE_IMPLEMENTATION m_impl;
};

} // namespace concurrency
} // namespace xmipp4

#include "counting_semaphore.inl"
