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
 * @file test_device_manager.cpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Test for device_manager.hpp
 * @date 2024-10-29
 */


#include <catch2/catch_test_macros.hpp>

#include <xmipp4/core/compute/device_manager.hpp>

#include <xmipp4/core/compute/device_backend.hpp>
#include <xmipp4/core/version.hpp>

using namespace xmipp4;
using namespace xmipp4::compute;

class test_device_backend
    : public device_backend
{
public:
    test_device_backend(std::string name)
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

    void enumerate_devices(std::vector<std::size_t> &ids) const final
    {
        ids = { 1, 2, 3, 4 };
    }

    bool get_device_properties(std::size_t, device_properties&) const final
    {
        return true;
    }

    std::unique_ptr<device> create_device(std::size_t) final
    {
        return nullptr;
    }

    std::shared_ptr<device> create_device_shared(std::size_t) final
    {
        return nullptr;
    }

private:
    std::string m_name;

};


TEST_CASE( "register device backend", "[device_manager]" ) 
{
    device_manager manager;
    manager.register_backend(std::make_unique<test_device_backend>("test1"));
    manager.register_backend(std::make_unique<test_device_backend>("test2"));

    std::vector<std::string> backends;
    manager.enumerate_backends(backends);

    REQUIRE( backends.size() == 2 );
    std::sort(backends.begin(), backends.end()); // Ordering not defined
    REQUIRE( backends[0] == "test1" );
    REQUIRE( backends[1] == "test2" );
}

TEST_CASE( "query device backend", "[device_manager]" ) 
{
    device_manager manager;
    manager.register_backend(std::make_unique<test_device_backend>("test1"));
    manager.register_backend(std::make_unique<test_device_backend>("test2"));


    device_backend* backend;
    backend = manager.get_backend("test1");
    REQUIRE( backend != nullptr );
    REQUIRE( backend->get_name() == "test1" );

    backend = manager.get_backend("test2");
    REQUIRE( backend != nullptr );
    REQUIRE( backend->get_name() == "test2" );

    REQUIRE( manager.get_backend("not-a-backend") == nullptr );
}

TEST_CASE( "register the same device backend twice", "[device_manager]" ) 
{
    device_manager manager;
    manager.register_backend(std::make_unique<test_device_backend>("test"));
    manager.register_backend(std::make_unique<test_device_backend>("test"));

    std::vector<std::string> backends;
    manager.enumerate_backends(backends);

    REQUIRE( backends.size() == 1 );
    REQUIRE( backends[0] == "test" );
}

TEST_CASE( "enumerate devices", "[device_manager]" ) 
{
    device_manager manager;
    manager.register_backend(std::make_unique<test_device_backend>("hip"));
    manager.register_backend(std::make_unique<test_device_backend>("cuda"));
    
    std::vector<device_index> devices;
    manager.enumerate_devices(devices);
    std::sort(devices.begin(), devices.end()); // Ordering not defined. 

    REQUIRE( devices.size() == 8 );
    REQUIRE( devices[0] == device_index("cuda", 1) );
    REQUIRE( devices[1] == device_index("cuda", 2) );
    REQUIRE( devices[2] == device_index("cuda", 3) );
    REQUIRE( devices[3] == device_index("cuda", 4) );
    REQUIRE( devices[4] == device_index("hip", 1) );
    REQUIRE( devices[5] == device_index("hip", 2) );
    REQUIRE( devices[6] == device_index("hip", 3) );
    REQUIRE( devices[7] == device_index("hip", 4) );
}
