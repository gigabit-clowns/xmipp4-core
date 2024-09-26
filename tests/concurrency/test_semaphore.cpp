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

/**
 * @file semaphore.cpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Tests for concurrency/semaphore.hpp
 * @date 2024-09-25
 * 
 */

#include <catch2/catch_test_macros.hpp>

#include <xmipp4/core/concurrency/semaphore.hpp>

TEST_CASE("acquire and release semaphore", "[counting_semaphore]")
{
    const auto n = 8;
    xmipp4::concurrency::counting_semaphore sem(n);
    
    for (std::size_t i = 0; i < n; ++i)
    {
        sem.acquire();
    }
    for (std::size_t i = 0; i < n; ++i)
    {
        sem.release();
    }
    for (std::size_t i = 0; i < n; ++i)
    {
        sem.acquire();
    }
    for (std::size_t i = 0; i < n; ++i)
    {
        sem.release();
    }
}

TEST_CASE("try acquire semaphore without timeout", "[counting_semaphore]")
{
    const auto n = 8;
    xmipp4::concurrency::counting_semaphore sem(n);
    
    // Acquire as many times as the semaphore allows
    for (std::size_t i = 0; i < n; ++i)
    {
        REQUIRE( sem.try_acquire() );
    }

    // Try to acquire beyond limits
    REQUIRE( !sem.try_acquire() );

    // Release to enable future acquisitions
    sem.release();

    REQUIRE( sem.try_acquire() );

    // Release all
    for (std::size_t i = 0; i < n; ++i)
    {
        sem.release();
    }
}

TEST_CASE("try acquire semaphore with absolute timeout", "[counting_semaphore]")
{
    const auto n = 8;
    xmipp4::concurrency::counting_semaphore sem(n);
    
    // Acquire as many times as the semaphore allows
    for (std::size_t i = 0; i < n; ++i)
    {
        REQUIRE( sem.try_acquire() );
    }

    // Try to acquire beyond limits
    const auto now = std::chrono::steady_clock::now();
    const auto timeout = now + std::chrono::milliseconds(500);
    REQUIRE( !sem.try_acquire_until(timeout) ); // Should wait
    REQUIRE( timeout <= std::chrono::steady_clock::now() );

    const auto past_timeout = now - std::chrono::milliseconds(500);
    REQUIRE( !sem.try_acquire_until(past_timeout) ); // Should instantly return

    sem.release();
    REQUIRE( sem.try_acquire_until(timeout) );

    // Release all
    for (std::size_t i = 0; i < n; ++i)
    {
        sem.release();
    }
}

TEST_CASE("try acquire semaphore with relative timeout", "[counting_semaphore]")
{
    const auto n = 8;
    xmipp4::concurrency::counting_semaphore sem(n);
    
    // Acquire as many times as the semaphore allows
    for (std::size_t i = 0; i < n; ++i)
    {
        REQUIRE( sem.try_acquire() );
    }

    // Try to acquire beyond limits
    const auto now = std::chrono::steady_clock::now();
    const auto timeout = std::chrono::milliseconds(500);
    REQUIRE( !sem.try_acquire_for(timeout) ); // Should wait
    // REQUIRE( (now + timeout) <= std::chrono::steady_clock::now() ); // Unreliable

    const auto past_timeout = -std::chrono::milliseconds(500);
    REQUIRE( !sem.try_acquire_for(past_timeout) ); // Should instantly return

    sem.release();
    REQUIRE( sem.try_acquire_for(timeout) );

    // Release all
    for (std::size_t i = 0; i < n; ++i)
    {
        sem.release();
    }
}
