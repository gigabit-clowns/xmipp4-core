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
 * @file test_device_communicator_manager.cpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Test for device_communicator_manager.hpp
 * @date 2024-10-29
 */


#include <catch2/catch_test_macros.hpp>

#include <xmipp4/core/compute/device_communicator_manager.hpp>

#include <xmipp4/core/compute/device_communicator_backend.hpp>
#include <xmipp4/core/compute/device.hpp>
#include <xmipp4/core/compute/device_buffer.hpp>
#include <xmipp4/core/compute/device_queue.hpp>
#include <xmipp4/core/compute/device_communicator.hpp>
#include <xmipp4/core/version.hpp>

#include <algorithm>

using namespace xmipp4;
using namespace xmipp4::compute;



class test_device1
    : public device
{
public:
    test_device1(std::string name)
        : m_name(std::move(name))
    {
    }

    const std::string& get_name() const noexcept
    {
        return m_name;
    }

    std::unique_ptr<device_queue> create_queue() final
    {
        return nullptr;
    }

    std::shared_ptr<device_queue> create_queue_shared() final
    {
        return nullptr;
    }

    std::unique_ptr<device_buffer> 
    create_buffer(numerical_type type, std::size_t count) final
    {
        return nullptr;
    }

    std::shared_ptr<device_buffer> 
    create_buffer_shared(numerical_type type, std::size_t count) final
    {
        return nullptr;
    }

private:
    std::string m_name;

};

class test_device2
    : public device
{
    std::unique_ptr<device_queue> create_queue() final
    {
        return nullptr;
    }

    std::shared_ptr<device_queue> create_queue_shared() final
    {
        return nullptr;
    }

    std::unique_ptr<device_buffer> 
    create_buffer(numerical_type type, std::size_t count) final
    {
        return nullptr;
    }

    std::shared_ptr<device_buffer> 
    create_buffer_shared(numerical_type type, std::size_t count) final
    {
        return nullptr;
    }
};


class test_device_communicator_backend
    : public device_communicator_backend
{
public:
    test_device_communicator_backend(std::string name)
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

    bool supports_device(const device &dev) const noexcept final
    {
        bool result;
        
        const auto* test_device = dynamic_cast<const test_device1*>(&dev);
        if (test_device)
        {
            result = test_device->get_name() == m_name;
        }
        else
        {
            result = false;
        }

        return result;
    }

    virtual std::unique_ptr<device_communicator> 
    create_communicator(device &dev,
                        const std::shared_ptr<host_communicator> &comm) const final
    {
        return nullptr;
    }

    virtual std::shared_ptr<device_communicator> 
    create_communicator_shared(device &dev,
                               const std::shared_ptr<host_communicator> &comm) const
    {
        return nullptr;
    }

private:
    std::string m_name;

};


TEST_CASE( "register device_communicator_backend", "[device_communicator_manager]" ) 
{
    device_communicator_manager manager;
    manager.register_backend(std::make_unique<test_device_communicator_backend>("test1"));
    manager.register_backend(std::make_unique<test_device_communicator_backend>("test2"));

    std::vector<std::string> backends;
    manager.enumerate_backends(backends);

    REQUIRE( backends.size() == 2 );
    std::sort(backends.begin(), backends.end()); // Ordering not defined
    REQUIRE( backends[0] == "test1" );
    REQUIRE( backends[1] == "test2" );
}

TEST_CASE( "query device_communicator_backend", "[device_communicator_manager]" ) 
{
    device_communicator_manager manager;
    manager.register_backend(std::make_unique<test_device_communicator_backend>("test1"));
    manager.register_backend(std::make_unique<test_device_communicator_backend>("test2"));

    device_communicator_backend* backend;
    backend = manager.get_backend("test1");
    REQUIRE( backend != nullptr );
    REQUIRE( backend->get_name() == "test1" );

    backend = manager.get_backend("test2");
    REQUIRE( backend != nullptr );
    REQUIRE( backend->get_name() == "test2" );

    REQUIRE( manager.get_backend("not-a-backend") == nullptr );
}

TEST_CASE( "register the same host device_communicator_backend twice", "[device_communicator_manager]" ) 
{
    device_communicator_manager manager;
    manager.register_backend(std::make_unique<test_device_communicator_backend>("test"));
    manager.register_backend(std::make_unique<test_device_communicator_backend>("test"));

    std::vector<std::string> backends;
    manager.enumerate_backends(backends);

    REQUIRE( backends.size() == 1 );
    REQUIRE( backends[0] == "test" );
}

TEST_CASE( "find a compatible device_communicator_backend", "[device_communicator_manager]" ) 
{
    device_communicator_manager manager;
    manager.register_backend(std::make_unique<test_device_communicator_backend>("test1"));
    manager.register_backend(std::make_unique<test_device_communicator_backend>("test2"));

    const auto* backend1 = manager.get_backend("test1");
    test_device1 backend1_compatible_device("test1");
    REQUIRE( manager.find_compatible_backend(backend1_compatible_device) == backend1 );
    
    const auto* backend2 = manager.get_backend("test2");
    test_device1 backend2_compatible_device("test2");
    REQUIRE( manager.find_compatible_backend(backend2_compatible_device) == backend2 );

    test_device2 incompatible_device;
    REQUIRE( manager.find_compatible_backend(incompatible_device) == nullptr );
}
