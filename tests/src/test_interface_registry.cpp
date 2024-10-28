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
 * @file test_interface_registry.cpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Tests for interface_registry.hpp
 * @date 2024-10-28
 * 
 */

#include <catch2/catch_test_macros.hpp>

#include <xmipp4/core/interface_registry.hpp>

using namespace xmipp4;

#define SAMPLE_MESSAGE "Hi, I am a dummy interface manager"

class dummy_interface_manager
    : public interface_manager
{
public:
    std::string message;

};

TEST_CASE( "get interface manager", "[interface_registry]" ) 
{
    interface_registry registry;

    auto& manager1 = registry.get_interface_manager<dummy_interface_manager>();
    manager1.message = SAMPLE_MESSAGE;

    auto& manager2 = registry.get_interface_manager<dummy_interface_manager>();
    REQUIRE( &manager1 == &manager2 );
    REQUIRE( manager2.message == SAMPLE_MESSAGE );
}
