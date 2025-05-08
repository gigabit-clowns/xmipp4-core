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
 * @file test_interface_catalog.cpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Tests for interface_catalog.hpp
 * @date 2024-10-28
 * 
 */

#include <xmipp4/core/interface_catalog.hpp>

#include "mock/mock_backend_manager.hpp"

#include <catch2/catch_test_macros.hpp>
#include <trompeloeil/matcher/any.hpp>

using namespace xmipp4;

TEST_CASE( "get_backend_manager should always return the same instance", "[interface_catalog]" ) 
{
    interface_catalog catalog(false);

    auto& manager1 = catalog.get_backend_manager<mock_backend_manager>();
    auto& manager2 = catalog.get_backend_manager<mock_backend_manager>();
    REQUIRE( &manager1 == &manager2 );
}
