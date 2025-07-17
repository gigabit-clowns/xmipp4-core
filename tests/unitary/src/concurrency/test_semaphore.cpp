// SPDX-License-Identifier: GPL-3.0-only

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

    const auto past_timeout = -std::chrono::milliseconds(500);

    sem.release();
    REQUIRE( sem.try_acquire_for(past_timeout) );

    // Release all
    for (std::size_t i = 0; i < n; ++i)
    {
        sem.release();
    }
}
