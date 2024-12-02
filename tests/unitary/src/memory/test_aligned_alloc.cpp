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
 * @file test_aligned_alloc.cpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Tests for memory/aligned_alloc.hpp
 * @date 2024-12-02
 * 
 */


#include <xmipp4/core/memory/aligned_alloc.hpp>

#include <xmipp4/core/memory/align.hpp>

#include <catch2/catch_test_macros.hpp>

using namespace xmipp4;

TEST_CASE( "aligned_alloc should produce aligned allocations", "[aligned_alloc]" ) 
{
	const std::size_t repetitions = 8192;
	const std::size_t alignment = 2048; // Large enough power of two.
	const std::size_t size = 1024;

	for (std::size_t i = 0; i < repetitions; ++i)
	{
		auto* data = memory::aligned_alloc(size, alignment);
		REQUIRE( data != nullptr ); // Assuming no OOM situation. 
		REQUIRE( memory::get_alignment(data) >= alignment );
		memory::aligned_free(data);
	}

}
