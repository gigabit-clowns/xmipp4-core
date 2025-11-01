// SPDX-License-Identifier: GPL-3.0-only

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers.hpp>

#include <hardware/host_memory_transfer.hpp>
#include <hardware/host_buffer.hpp>
#include <xmipp4/core/hardware/copy_region.hpp>

#include "mock/mock_device_queue.hpp"
#include "mock/mock_buffer.hpp"

#include <sstream>


using namespace xmipp4::hardware;

TEST_CASE( "copy in host_memory_transfer should successfully copy memory ranges", "[host_memory_transfer]" )
{
    using U = std::uint64_t;
    host_memory_transfer transfer;
    const auto source_element_count = 256;
    const auto source_byte_count = source_element_count * sizeof(U);
    const auto destination_element_count = 512;
    const auto destination_byte_count = destination_element_count * sizeof(U);
    host_buffer source(source_byte_count, alignof(U));
    host_buffer destination(destination_byte_count, alignof(U));

    auto* source_ptr = static_cast<U*>(source.get_host_ptr());
    for (std::size_t i = 0; i < source_element_count; ++i)
    {
        source_ptr[i] = static_cast<U>(i);
    }

    const U destination_fill_value = 0xDEADBEEF;
    auto destination_ptr = static_cast<U*>(destination.get_host_ptr());
    for (std::size_t i = 0; i < destination_element_count; ++i)
    {
        destination_ptr[i] = destination_fill_value;
    }

    const std::array<copy_region, 3> regions = {{
        copy_region(0 * sizeof(U), 0 * sizeof(U), 64 * sizeof(U)),
        copy_region(128 * sizeof(U), 480 * sizeof(U), 32 * sizeof(U)),
        copy_region(192 * sizeof(U), 128 * sizeof(U), 64 * sizeof(U))
    }};

    transfer.copy(source, destination, xmipp4::make_span(regions), nullptr);

    for (std::size_t i = 0; i < 64; ++i)
    {
        REQUIRE( destination_ptr[i] == i );
    }
    for (std::size_t i = 64; i < 128; ++i)
    {
        REQUIRE( destination_ptr[i] == destination_fill_value );
    }
    for (std::size_t i = 128; i < 192; ++i)
    {
        REQUIRE( destination_ptr[i] == i+64 );
    }
    for (std::size_t i = 192; i < 480; ++i)
    {
        REQUIRE( destination_ptr[i] == destination_fill_value );
    }
    for (std::size_t i = 480; i < 512; ++i)
    {
        REQUIRE( destination_ptr[i] == i-352 );
    }

}

TEST_CASE( "copy in host_memory_transfer with a queue should synchronize", "[host_memory_transfer]" )
{
    using U = std::uint32_t;
    host_memory_transfer transfer;
    host_buffer source(1024, 16);
    host_buffer destination(1024, 16);

    mock_device_queue queue;
    REQUIRE_CALL(queue, wait_until_completed());

    const std::array<copy_region, 1> regions = {{
        copy_region(0 * sizeof(U), 0 * sizeof(U), 64 * sizeof(U)),
    }};

    transfer.copy(source, destination, xmipp4::make_span(regions), &queue);

}

TEST_CASE( "copy in host_memory_transfer should throw if the source is not host accessible", "[host_memory_transfer]" )
{
    host_memory_transfer transfer;
    mock_buffer source;
    host_buffer destination(1024, 16);

    const auto &const_source = source;
    REQUIRE_CALL(const_source, get_host_ptr())
        .TIMES(2)
        .RETURN(nullptr);

    const std::array<copy_region, 1> regions = {{
        copy_region(0, 0, 64),
    }};

    REQUIRE_THROWS_AS(transfer.copy(source, destination, xmipp4::make_span(regions), nullptr), std::invalid_argument);
    REQUIRE_THROWS_WITH(transfer.copy(source, destination, xmipp4::make_span(regions), nullptr), "Source buffer is not host accessible.");

}

TEST_CASE( "copy in host_memory_transfer should throw if the destination is not host accessible", "[host_memory_transfer]" )
{
    host_memory_transfer transfer;
    mock_buffer destination;
    host_buffer source(1024, 16);

    REQUIRE_CALL(destination, get_host_ptr())
        .TIMES(2)
        .RETURN(nullptr);

    const std::array<copy_region, 1> regions = {{
        copy_region(0, 0, 64),
    }};

    REQUIRE_THROWS_AS(transfer.copy(source, destination, xmipp4::make_span(regions), nullptr), std::invalid_argument);
    REQUIRE_THROWS_WITH(transfer.copy(source, destination, xmipp4::make_span(regions), nullptr), "Destination buffer is not host accessible.");

}

TEST_CASE( "copy in host_memory_transfer should throw if a source region exceeds bounds ", "[host_memory_transfer]" )
{
    host_memory_transfer transfer;
    host_buffer destination(1024, 16);
    host_buffer source(2048, 16);

    const std::array<copy_region, 2> regions = {{
        copy_region(0, 0, 64),
        copy_region(1025, 0, 1024),
    }};

    REQUIRE_THROWS_AS(transfer.copy(source, destination, xmipp4::make_span(regions), nullptr), std::out_of_range);
    REQUIRE_THROWS_WITH(transfer.copy(source, destination, xmipp4::make_span(regions), nullptr), "Copy region exceeds source buffer size.");

}

TEST_CASE( "copy in host_memory_transfer should throw if a destination region exceeds bounds ", "[host_memory_transfer]" )
{
    host_memory_transfer transfer;
    host_buffer destination(1024, 16);
    host_buffer source(2048, 16);

    const std::array<copy_region, 2> regions = {{
        copy_region(0, 0, 64),
        copy_region(1024, 1, 1024),
    }};

    REQUIRE_THROWS_AS(transfer.copy(source, destination, xmipp4::make_span(regions), nullptr), std::out_of_range);
    REQUIRE_THROWS_WITH(transfer.copy(source, destination, xmipp4::make_span(regions), nullptr), "Copy region exceeds destination buffer size.");

}
