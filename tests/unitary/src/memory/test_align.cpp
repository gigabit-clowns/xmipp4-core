// SPDX-License-Identifier: GPL-3.0-only

#include <catch2/catch_test_macros.hpp>

#include <xmipp4/core/memory/align.hpp>

using namespace xmipp4;

TEST_CASE( "get alignment", "[align]" ) 
{
	const std::uintptr_t offset = 0xABCD0100;
	REQUIRE( memory::get_alignment(offset) == 0x100 );

	void* pointer = reinterpret_cast<void*>(offset);
	REQUIRE( memory::get_alignment(pointer) == 0x100 );
}

TEST_CASE( "align floor", "[align]" ) 
{
	const std::uintptr_t offset = 0xA1010100;
	REQUIRE( memory::align_floor(offset, 0x10) == offset );
	REQUIRE( memory::align_floor(offset, 0x100) == offset );
	REQUIRE( memory::align_floor(offset, 0x1000) == 0xA1010000 );

	void* pointer = reinterpret_cast<void*>(offset);
	REQUIRE( memory::align_floor(pointer, 0x10) == pointer );
	REQUIRE( memory::align_floor(pointer, 0x100) == pointer );
	REQUIRE( memory::align_floor(pointer, 0x1000) == reinterpret_cast<void*>(uintptr_t(0xA1010000)) );
}

TEST_CASE( "align ceil", "[align]" ) 
{
	const std::uintptr_t offset = 0xA1010100;
	REQUIRE( memory::align_ceil(offset, 0x10) == offset );
	REQUIRE( memory::align_ceil(offset, 0x100) == offset );
	REQUIRE( memory::align_ceil(offset, 0x1000) == 0xA1011000 );

	void* pointer = reinterpret_cast<void*>(offset);
	REQUIRE( memory::align_ceil(pointer, 0x10) == pointer );
	REQUIRE( memory::align_ceil(pointer, 0x100) == pointer );
	REQUIRE( memory::align_ceil(pointer, 0x1000) == reinterpret_cast<void*>(uintptr_t(0xA1011000)) );
}

TEST_CASE( "offset_bytes", "[align]" ) 
{
	std::uint32_t data;
	std::uint32_t* next = memory::offset_bytes(&data, sizeof(std::uint32_t));
	REQUIRE( next == &data+1 );
}
