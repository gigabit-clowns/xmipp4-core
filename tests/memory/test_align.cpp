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
 * @file test_align.cpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Tests for memory/align.hpp
 * @date 2024-03-10
 * 
 */

#include <catch2/catch_test_macros.hpp>

#include <xmipp4/memory/align.hpp>

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
	REQUIRE( memory::align_floor(pointer, 0x1000) == reinterpret_cast<void*>(0xA1010000) );
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
	REQUIRE( memory::align_ceil(pointer, 0x1000) == reinterpret_cast<void*>(0xA1011000) );
}
