// SPDX-License-Identifier: GPL-3.0-only

#include "../assets.hpp"

#include <xmipp4/core/system/dynamic_library.hpp>

#include <catch2/catch_test_macros.hpp>

#include <sstream>

using namespace xmipp4;

TEST_CASE( "open a dynamic library", "[dynamic_library]" ) 
{
	system::dynamic_library dl(get_mock_plugin_path("dummy-plugin"));

	REQUIRE( dl.is_open() );
	REQUIRE( dl.get_symbol("lorem_ipsum") == nullptr );

	const auto *symbol = dl.get_symbol("xmipp4_get_plugin");
	REQUIRE( symbol != nullptr );
}

TEST_CASE( "default construct dynamic_library", "[dynamic_library]" ) 
{
	system::dynamic_library lib;
	REQUIRE( lib.is_open() == false );
}

TEST_CASE( "recover shared library path from symbol", "[dynamic_library]" ) 
{
	// Open a dynamic library and query a symbol.
	const auto path = get_mock_plugin_path("dummy-plugin");
	system::dynamic_library dl(path);
	const auto *symbol = dl.get_symbol("xmipp4_get_plugin");

	// Recover path from loaded symbol
	const auto path2 = system::dynamic_library::query_symbol_filename(symbol);

	// The last part of the path should coincide even if the returned
	// path is absolute
	REQUIRE( path2.ends_with(path) );
}
