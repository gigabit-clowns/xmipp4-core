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
 * @file test_host_communicator_manager.cpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Test for host_communicator_manager.hpp
 * @date 2024-10-29
 */


#include <catch2/catch_test_macros.hpp>

#include <xmipp4/core/compute/host_communicator_manager.hpp>

#include <xmipp4/core/compute/host_communicator_backend.hpp>
#include <xmipp4/core/version.hpp>

#include <algorithm>

using namespace xmipp4;
using namespace xmipp4::compute;

class test_host_communicator_backend
    : public host_communicator_backend
{
public:
    test_host_communicator_backend(std::string name)
        : m_name(std::move(name))
    {
    }

    const std::string& get_name() const noexcept final
    {
        return m_name;
    }

    version get_version() const noexcept final
    {
        return version(2, 4, 5);
    }

    bool is_available() const noexcept final
    {
        return true;
    }

    std::shared_ptr<host_communicator> 
    get_world_communicator() const final
    {
        return nullptr;
    }

private:
    std::string m_name;

};


TEST_CASE( "register host communicator backend", "[host_communicator_manager]" ) 
{
    host_communicator_manager manager;
    manager.register_backend(std::make_unique<test_host_communicator_backend>("test1"));
    manager.register_backend(std::make_unique<test_host_communicator_backend>("test2"));

    std::vector<std::string> backends;
    manager.enumerate_backends(backends);

    REQUIRE( backends.size() == 2 );
    std::sort(backends.begin(), backends.end()); // Ordering not defined
    REQUIRE( backends[0] == "test1" );
    REQUIRE( backends[1] == "test2" );
}

TEST_CASE( "query host communicator backend", "[host_communicator_manager]" ) 
{
    host_communicator_manager manager;
    manager.register_backend(std::make_unique<test_host_communicator_backend>("test1"));
    manager.register_backend(std::make_unique<test_host_communicator_backend>("test2"));

    host_communicator_backend* backend;
    backend = manager.get_backend("test1");
    REQUIRE( backend != nullptr );
    REQUIRE( backend->get_name() == "test1" );

    backend = manager.get_backend("test2");
    REQUIRE( backend != nullptr );
    REQUIRE( backend->get_name() == "test2" );

    REQUIRE( manager.get_backend("not-a-backend") == nullptr );
}

TEST_CASE( "register the same host communicator backend twice", "[host_communicator_manager]" ) 
{
    host_communicator_manager manager;
    manager.register_backend(std::make_unique<test_host_communicator_backend>("test"));
    manager.register_backend(std::make_unique<test_host_communicator_backend>("test"));

    std::vector<std::string> backends;
    manager.enumerate_backends(backends);

    REQUIRE( backends.size() == 1 );
    REQUIRE( backends[0] == "test" );
}
