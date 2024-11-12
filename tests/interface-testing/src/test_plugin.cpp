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
 * @file test_plugin.cpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Implementation of tests for plugin
 * @date 2024-11-12
 * 
 */

#include <test_plugin.hpp>

#include <xmipp4/core/plugin.hpp>
#include <xmipp4/core/plugin_manager.hpp>
#include <xmipp4/core/platform/operating_system.h>

#include <catch2/catch_test_macros.hpp>

namespace xmipp4
{
namespace testing
{

static std::string get_plugin_path(const std::string &plugin_name)
{
    #if XMIPP4_WINDOWS
        return plugin_name + ".dll";
    #elif XMIPP4_APPLE || XMIPP4_LINUX
        return "./lib" + plugin_name + ".so";
    #else
        #error "Unknown platform"
    #endif
}

void test_plugin_load(const std::string &name)
{
    plugin_manager manager;

    const auto* plugin = 
        manager.load_plugin(get_plugin_path(name));

    REQUIRE( plugin != nullptr );
    REQUIRE( plugin->get_name() == name );
}

} // namespace testing
} // namespace xmipp4
