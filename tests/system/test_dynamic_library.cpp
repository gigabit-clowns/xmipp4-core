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

#include <xmipp4/xmipp.hpp>
#include <xmipp4/platform/operating_system.h>

#include <sstream>

using namespace xmipp4;

static std::string get_xmipp_core_soname()
{
    #if defined(XMIPP4_LINUX)
        std::stringstream stream;
        stream << "./libxmipp4-core.so." << get_core_version();
        return stream.str();
    #elif defined(XMIPP4_APPLE)
        return "xmipp4-core.dylib"
    #elif defined(XMIPP4_WINDOWS)
        return "xmipp4-core"
    #else
        #error "Unknown OS"
    #endif
}

TEST_CASE( "open xmipp4-core as dynamic library", "[dynamic_library]" ) 
{
    system::dynamic_library xmipp4_core(get_xmipp_core_soname());

    REQUIRE( xmipp4_core.is_open() );
    REQUIRE( xmipp4_core.get_symbol("Lorem_ipsum") == nullptr );

    using test_hook_function_ptr =  uint32_t (*)();
    const auto test_hook= reinterpret_cast<test_hook_function_ptr>( 
        xmipp4_core.get_symbol("xmipp4_dynamic_library_test_hook")
    );

    REQUIRE(test_hook != nullptr );
    REQUIRE(test_hook() == 0xDEADBEEF );
}

TEST_CASE( "default construct dynamic_library", "[dynamic_library]" ) 
{
    system::dynamic_library lib;
    REQUIRE( lib.is_open() == false );
}
