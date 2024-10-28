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

#include "../assets.hpp"

#include <xmipp4/core/system/dynamic_library.hpp>

#include <catch2/catch_test_macros.hpp>

#include <sstream>

using namespace xmipp4;

TEST_CASE( "open a dynamic library", "[dynamic_library]" ) 
{
    system::dynamic_library xmipp4_core(get_mock_plugin_path("dummy_plugin"));

    REQUIRE( xmipp4_core.is_open() );
    REQUIRE( xmipp4_core.get_symbol("lorem_ipsum") == nullptr );

    const auto *symbol = xmipp4_core.get_symbol("xmipp4_get_plugin");
    REQUIRE( symbol != nullptr );
}

TEST_CASE( "default construct dynamic_library", "[dynamic_library]" ) 
{
    system::dynamic_library lib;
    REQUIRE( lib.is_open() == false );
}
