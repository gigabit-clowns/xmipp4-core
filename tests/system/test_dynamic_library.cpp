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
 * @file test_dynamic_library.cpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Tests for system/dynamic_library.hpp
 * @date 2024-03-03
 * 
 */

#include <catch2/catch_test_macros.hpp>

#include <xmipp4/system/dynamic_library.hpp>

#include <xmipp4/platform/operating_system.h>

using namespace xmipp4;

TEST_CASE( "open libc as dynamic library", "[dynamic_library]" ) 
{
    #if defined(XMIPP4_LINUX)
        system::dynamic_library libc("libc.so.6");
    #elif defined(XMIPP4_APPLE)
        system::dynamic_library libc("libc.dylib");
    #elif defined(XMIPP4_WINDOWS)
        system::dynamic_library libc("libc.dll");
    #else
        #error "Unknown OS"
    #endif

    REQUIRE( libc.is_open() );
    REQUIRE( libc.get_symbol("fopen") != nullptr );
}

