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
 * @file test_plugin_manager.cpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Tests for plugin_manager.hpp
 * @date 2024-10-28
 * 
 */

#include <catch2/catch_test_macros.hpp>

#include <xmipp4/core/plugin.hpp>
#include <xmipp4/core/plugin_manager.hpp>
#include <xmipp4/core/platform/operating_system.h>

using namespace xmipp4;

static std::string make_sopath(const std::string &directory, const std::string &module)
{
    #if XMIPP4_WINDOWS
        return directory + "/" + module;
    #elif XMIPP4_APPLE || XMIPP4_LINUX
        return directory + "/lib" + module + ".so";
    #endif
}

TEST_CASE( "load good plugin in the plugin manager", "[plugin_manager]" ) 
{
    plugin_manager manager;
    const auto* plugin = manager.load_plugin(make_sopath("assets/dummy_plugin", "dummy_plugin"));
    REQUIRE( plugin->get_name() == "dummy-plugin" );
    REQUIRE( plugin->get_version() == version(1, 2, 3) );
    REQUIRE( manager.get_plugin_count() == 1 );
    REQUIRE( &(manager.get_plugin(0)) == plugin );
}

TEST_CASE( "load invalid plugin in the plugin manager", "[plugin_manager]" ) 
{
    plugin_manager manager;
    REQUIRE_THROWS( manager.load_plugin("path/to/nowhere") );
    REQUIRE( manager.get_plugin_count() == 0 );
    REQUIRE_THROWS( manager.load_plugin("assets/lorem_ipsum.txt") );
    REQUIRE( manager.get_plugin_count() == 0 );
    REQUIRE_THROWS( manager.load_plugin(make_sopath("assets/faulty_plugin1", "faulty_plugin1")) );
    REQUIRE( manager.get_plugin_count() == 0 );
    REQUIRE_THROWS( manager.load_plugin(make_sopath("assets/faulty_plugin2", "faulty_plugin2")) );
    REQUIRE( manager.get_plugin_count() == 0 );
}
