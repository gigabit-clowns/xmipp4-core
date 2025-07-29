// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/compute/unified_copy.hpp>

#include "mock/mock_unified_copy.hpp"
#include "mock/mock_unified_buffer.hpp"
#include "mock/mock_unified_memory_allocator.hpp"
#include "mock/mock_device_queue.hpp"

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers.hpp>

using namespace xmipp4;
using namespace xmipp4::compute;

TEST_CASE( "default implementation of host to device transfer in unified_copy should return an alias", "[unified_buffer]" )
{
    unified_copy copy;

    mock_device_queue queue;
    mock_unified_memory_allocator allocator;
    const std::shared_ptr<host_buffer> src_buffer = std::make_shared<mock_unified_buffer>();
    const auto alignment = 1;

    const auto dst_buffer = copy.transfer(src_buffer, allocator, alignment, queue);

    REQUIRE(  static_cast<unified_buffer*>(src_buffer.get()) == static_cast<unified_buffer*>(dst_buffer.get()) );
}

TEST_CASE( "default implementation of host to device transfer in unified_copy should return a const alias", "[unified_buffer]" )
{
    unified_copy copy;

    mock_device_queue queue;
    mock_unified_memory_allocator allocator;
    const std::shared_ptr<const host_buffer> src_buffer = std::make_shared<mock_unified_buffer>();
    const auto alignment = 1;

    const auto dst_buffer = copy.transfer(src_buffer, allocator, alignment, queue);

    REQUIRE(  static_cast<const unified_buffer*>(src_buffer.get()) == static_cast<const unified_buffer*>(dst_buffer.get()) );
}

TEST_CASE( "default implementation of host to device transfer_copy should call unified_copy", "[unified_buffer]" )
{
    mock_unified_copy copy;
    mock_device_queue queue;
    mock_unified_buffer src_buffer;
    mock_unified_buffer dst_buffer;

    REQUIRE_CALL(copy, transfer_copy(ANY(const host_buffer&), ANY(device_buffer&), ANY(device_queue&)))
        .LR_SIDE_EFFECT(copy.unified_copy::transfer_copy(_1, _2, _3));

    REQUIRE_CALL(copy, copy_unified(ANY(const unified_buffer&), ANY(unified_buffer&), ANY(device_queue&)))
        .LR_WITH(&_1 == &src_buffer )
        .LR_WITH(&_2 == &dst_buffer )
        .LR_WITH(&_3 == &queue );

    copy.transfer_copy(
        static_cast<const host_buffer&>(src_buffer), 
        dst_buffer, 
        queue
    );
}

TEST_CASE( "default implementation of host to device transfer_copy with regions should call unified_copy with regions", "[unified_buffer]" )
{
    mock_unified_copy copy;
    mock_device_queue queue;
    mock_unified_buffer src_buffer;
    mock_unified_buffer dst_buffer;
    const std::array<copy_region, 2> regions = {
        copy_region(512, 0, 512),
        copy_region(0, 1536, 512),
    };

    REQUIRE_CALL(copy, transfer_copy(ANY(const host_buffer&), ANY(device_buffer&), ANY(xmipp4::span<const copy_region>), ANY(device_queue&)))
        .LR_SIDE_EFFECT(copy.unified_copy::transfer_copy(_1, _2, _3, _4));

    REQUIRE_CALL(copy, copy_unified(ANY(const unified_buffer&), ANY(unified_buffer&), ANY(xmipp4::span<const copy_region>), ANY(device_queue&)))
        .LR_WITH(&_1 == &src_buffer )
        .LR_WITH(&_2 == &dst_buffer )
        .LR_WITH(_3.data() == regions.data())
        .LR_WITH(_3.size() == regions.size())
        .LR_WITH(&_4 == &queue );

    copy.transfer_copy(
        static_cast<const host_buffer&>(src_buffer), 
        dst_buffer, 
        xmipp4::make_span(regions), 
        queue
    );
}

TEST_CASE( "default implementation of device to host transfer in unified_copy should return an alias", "[unified_buffer]" )
{
    unified_copy copy;

    mock_device_queue queue;
    mock_unified_memory_allocator allocator;
    const std::shared_ptr<device_buffer> src_buffer = std::make_shared<mock_unified_buffer>();
    const auto alignment = 1;

    const auto dst_buffer = copy.transfer(src_buffer, allocator, alignment, queue);

    REQUIRE(  static_cast<unified_buffer*>(src_buffer.get()) == static_cast<unified_buffer*>(dst_buffer.get()) );
}

TEST_CASE( "default implementation of device to host transfer in unified_copy should return a const alias", "[unified_buffer]" )
{
    unified_copy copy;

    mock_device_queue queue;
    mock_unified_memory_allocator allocator;
    const std::shared_ptr<const device_buffer> src_buffer = std::make_shared<mock_unified_buffer>();
    const auto alignment = 1;

    const auto dst_buffer = copy.transfer(src_buffer, allocator, alignment, queue);

    REQUIRE(  static_cast<const unified_buffer*>(src_buffer.get()) == static_cast<const unified_buffer*>(dst_buffer.get()) );
}

TEST_CASE( "default implementation of device to host transfer_copy should call unified_copy", "[unified_buffer]" )
{
    mock_unified_copy copy;
    mock_device_queue queue;
    mock_unified_buffer src_buffer;
    mock_unified_buffer dst_buffer;

    REQUIRE_CALL(copy, transfer_copy(ANY(const device_buffer&), ANY(host_buffer&), ANY(device_queue&)))
        .LR_SIDE_EFFECT(copy.unified_copy::transfer_copy(_1, _2, _3));

    REQUIRE_CALL(copy, copy_unified(ANY(const unified_buffer&), ANY(unified_buffer&), ANY(device_queue&)))
        .LR_WITH(&_1 == &src_buffer )
        .LR_WITH(&_2 == &dst_buffer )
        .LR_WITH(&_3 == &queue );

    copy.transfer_copy(
        static_cast<const device_buffer&>(src_buffer), 
        dst_buffer, 
        queue
    );
}

TEST_CASE( "default implementation of device to host transfer_copy with regions should call unified_copy with regions", "[unified_buffer]" )
{
    mock_unified_copy copy;
    mock_device_queue queue;
    mock_unified_buffer src_buffer;
    mock_unified_buffer dst_buffer;
    const std::array<copy_region, 2> regions = {
        copy_region(512, 0, 512),
        copy_region(0, 1536, 512),
    };

    REQUIRE_CALL(copy, transfer_copy(ANY(const device_buffer&), ANY(host_buffer&), ANY(xmipp4::span<const copy_region>), ANY(device_queue&)))
        .LR_SIDE_EFFECT(copy.unified_copy::transfer_copy(_1, _2, _3, _4));

    REQUIRE_CALL(copy, copy_unified(ANY(const unified_buffer&), ANY(unified_buffer&), ANY(xmipp4::span<const copy_region>), ANY(device_queue&)))
        .LR_WITH(&_1 == &src_buffer )
        .LR_WITH(&_2 == &dst_buffer )
        .LR_WITH(_3.data() == regions.data())
        .LR_WITH(_3.size() == regions.size())
        .LR_WITH(&_4 == &queue );

    copy.transfer_copy(
        static_cast<const device_buffer&>(src_buffer), 
        dst_buffer, 
        xmipp4::make_span(regions), 
        queue
    );
}

TEST_CASE( "default implementation of device copy should call unified_copy", "[unified_buffer]" )
{
    mock_unified_copy copy;
    mock_device_queue queue;
    mock_unified_buffer src_buffer;
    mock_unified_buffer dst_buffer;

    REQUIRE_CALL(copy, copy(ANY(const device_buffer&), ANY(device_buffer&), ANY(device_queue&)))
        .LR_SIDE_EFFECT(copy.unified_copy::copy(_1, _2, _3));

    REQUIRE_CALL(copy, copy_unified(ANY(const unified_buffer&), ANY(unified_buffer&), ANY(device_queue&)))
        .LR_WITH(&_1 == &src_buffer )
        .LR_WITH(&_2 == &dst_buffer )
        .LR_WITH(&_3 == &queue );

    copy.copy(src_buffer, dst_buffer, queue);
}

TEST_CASE( "default implementation of device copy with regions should call unified_copy with regions", "[unified_buffer]" )
{
    mock_unified_copy copy;
    mock_device_queue queue;
    mock_unified_buffer src_buffer;
    mock_unified_buffer dst_buffer;
    const std::array<copy_region, 2> regions = {
        copy_region(512, 0, 512),
        copy_region(0, 1536, 512),
    };

    REQUIRE_CALL(copy, copy(ANY(const device_buffer&), ANY(device_buffer&), ANY(xmipp4::span<const copy_region>), ANY(device_queue&)))
        .LR_SIDE_EFFECT(copy.unified_copy::copy(_1, _2, _3, _4));

    REQUIRE_CALL(copy, copy_unified(ANY(const unified_buffer&), ANY(unified_buffer&), ANY(xmipp4::span<const copy_region>), ANY(device_queue&)))
        .LR_WITH(&_1 == &src_buffer )
        .LR_WITH(&_2 == &dst_buffer )
        .LR_WITH(_3.data() == regions.data())
        .LR_WITH(_3.size() == regions.size())
        .LR_WITH(&_4 == &queue );

    copy.copy(src_buffer, dst_buffer, xmipp4::make_span(regions), queue);
}
