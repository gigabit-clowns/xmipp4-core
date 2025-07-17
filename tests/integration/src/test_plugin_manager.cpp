// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/plugin_manager.hpp>

#include <xmipp4/core/plugin.hpp>
#include <xmipp4/core/exceptions/plugin_load_error.hpp>

#include "assets.hpp"

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers.hpp>

using namespace xmipp4;



TEST_CASE( "load good plugin in the plugin manager", "[plugin_manager]" ) 
{
    plugin_manager manager;
    const auto* plugin = manager.load_plugin(get_mock_plugin_path("dummy_plugin"));
    REQUIRE( plugin->get_name() == "dummy-plugin" );
    REQUIRE( plugin->get_version() == version(1, 2, 3) );
    REQUIRE( manager.get_plugin_count() == 1 );
    REQUIRE( &(manager.get_plugin(0)) == plugin );
}

TEST_CASE( "load invalid plugin in the plugin manager should throw", "[plugin_manager]" ) 
{
    plugin_manager manager;

    REQUIRE_THROWS_AS(
        manager.load_plugin("path_to_nowhere"), 
        std::system_error
    );
    REQUIRE( manager.get_plugin_count() == 0 );

    REQUIRE_THROWS_AS( 
        manager.load_plugin(get_text_file_path()), 
        std::system_error
    );
    REQUIRE( manager.get_plugin_count() == 0 );

    REQUIRE_THROWS_AS( 
        manager.load_plugin(get_mock_plugin_path("faulty_plugin1")), 
        plugin_load_error 
    );
    REQUIRE_THROWS_WITH( 
        manager.load_plugin(get_mock_plugin_path("faulty_plugin1")),
        "xmipp4_get_plugin returned NULL."
    );
    REQUIRE( manager.get_plugin_count() == 0 );

    REQUIRE_THROWS_AS( 
        manager.load_plugin(get_mock_plugin_path("faulty_plugin2")), 
        plugin_load_error
    );
    REQUIRE_THROWS_WITH( 
        manager.load_plugin(get_mock_plugin_path("faulty_plugin2")), 
        "xmipp4_get_plugin symbol could not be found in shared object."
    );
    REQUIRE( manager.get_plugin_count() == 0 );
}

TEST_CASE( "querying out out range plugin from plugin manager should throw", "[plugin_manager]" ) 
{
    plugin_manager manager;
    manager.load_plugin(get_mock_plugin_path("dummy_plugin"));
    REQUIRE_THROWS_AS( manager.get_plugin(1), std::out_of_range );
    REQUIRE_THROWS_AS( manager.get_plugin(10), std::out_of_range );
}

TEST_CASE( "discover_plugins should tolerate invalid plugins", "[plugin_manager]" ) 
{
    plugin_manager manager;
    REQUIRE_NOTHROW( discover_plugins(get_test_plugin_directory(), manager) );
    REQUIRE( manager.get_plugin_count() == 1 );

    const plugin& plugin = manager.get_plugin(0);
    REQUIRE( plugin.get_name() == "dummy-plugin" );
    REQUIRE( plugin.get_version() == version(1, 2, 3) );
}

TEST_CASE( "discover_plugins should tolerate non-existing directories", "[plugin_manager]" ) 
{
    plugin_manager manager;
    REQUIRE_NOTHROW( discover_plugins("/path/to/nowhere/dshfjbfnmxbcusdfj", manager) );
    REQUIRE( manager.get_plugin_count() == 0 );
}
