// SPDX-License-Identifier: GPL-3.0-only

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_exception.hpp>

#include <xmipp4/core/hardware/buffer.hpp>

#include <xmipp4/core/exceptions/invalid_operation_error.hpp>

#include "mock/mock_buffer_sentinel.hpp"
#include "mock/mock_memory_resource.hpp"
#include "mock/mock_device_queue.hpp"

#include <cstddef>

using namespace xmipp4::hardware;

TEST_CASE( "Constructing a buffer should store its attributes", "[buffer]" )
{
    void *host_ptr = reinterpret_cast<void*>(std::uintptr_t(0xDEADBEEF));
    const std::size_t size = 1024;
    mock_memory_resource resource;
    auto sentinel = std::make_unique<mock_buffer_sentinel>();
    auto *sentinel_ptr = sentinel.get();

    buffer buf(host_ptr, size, resource, std::move(sentinel));

    REQUIRE( buf.get_host_ptr() == host_ptr );
    REQUIRE( static_cast<const buffer&>(buf).get_host_ptr() == host_ptr );
    REQUIRE( buf.get_size() == size );
    REQUIRE( &buf.get_memory_resource() == &resource );
    REQUIRE( buf.get_sentinel() == sentinel_ptr );
    REQUIRE( static_cast<const buffer&>(buf).get_sentinel() == sentinel_ptr );
}

TEST_CASE( "Calling record_buffer in buffer should forward the call to the sentinel", "[buffer]" )
{
    mock_memory_resource resource;
    auto sentinel = std::make_unique<mock_buffer_sentinel>();
    auto *sentinel_ptr = sentinel.get();
    mock_device_queue queue;

    buffer buf(nullptr, 1024, resource, std::move(sentinel));

    REQUIRE_CALL(*sentinel_ptr, record_queue(ANY(device_queue&), true))
        .LR_WITH(&_1 == &queue);
    buf.record_queue(queue, true);

    REQUIRE_CALL(*sentinel_ptr, record_queue(ANY(device_queue&), false))
        .LR_WITH(&_1 == &queue);
    buf.record_queue(queue, false);
}

TEST_CASE( "Calling record_buffer a buffer without a sentinel should throw", "[buffer]" )
{
    mock_memory_resource resource;
    mock_device_queue queue;

    buffer buf(nullptr, 1024, resource, nullptr);

    REQUIRE_THROWS_MATCHES(
        buf.record_queue(queue, false),
		xmipp4::invalid_operation_error,
		Catch::Matchers::Message(
            "Can not call buffer::record_queue on a buffer without a sentinel"
        )
	);

}
