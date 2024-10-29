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
 * @file device_communicator_manager.cpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Implementation of device_communicator_manager.hpp
 * @date 2024-10-29
 * 
 */

#include <xmipp4/core/compute/device_communicator_manager.hpp>

#include <xmipp4/core/compute/device_communicator_backend.hpp>

#include <unordered_map>
#include <algorithm>

namespace xmipp4
{
namespace compute
{

class device_communicator_manager::implementation
{
public:
    bool register_backend(std::unique_ptr<device_communicator_backend> backend)
    {
        bool inserted = false;
        if (backend)
        {
            auto key = backend->get_name();
            std::tie(std::ignore, inserted) = m_registry.emplace(
                std::move(key), std::move(backend)
            );
        }

        return inserted;
    }

    void enumerate_backends(std::vector<std::string> &backends) const
    {
        backends.clear();
        backends.reserve(m_registry.size());

        for(const auto &backend : m_registry)
        {
            backends.push_back(backend.first);
        }
    }

    device_communicator_backend* get_backend(const std::string &name) const
    {
        const auto ite = m_registry.find(name);

        device_communicator_backend *result = nullptr;
        if (ite != m_registry.end())
        {
            result = ite->second.get();
        }

        return result;
    }

    device_communicator_backend* 
    find_compatible_backend(const device &dev) const
    {
        // Find a compatible backend
        const auto ite = std::find_if(
            m_registry.cbegin(), m_registry.cend(),
            [&dev] (const auto &backend) -> bool
            {
                return backend.second->supports_device(dev);
            }
        );

        device_communicator_backend* result = nullptr;
        if (ite != m_registry.cend())
        {
            result = ite->second.get();
        }

        return result;
    }

private:
    std::unordered_map<std::string, std::unique_ptr<device_communicator_backend>> m_registry;

};





device_communicator_manager::device_communicator_manager() = default;

device_communicator_manager::device_communicator_manager(device_communicator_manager &&other) = default;

device_communicator_manager::~device_communicator_manager() = default;

device_communicator_manager& 
device_communicator_manager::operator=(device_communicator_manager &&other) = default;



bool device_communicator_manager::register_backend(std::unique_ptr<device_communicator_backend> backend)
{
    return m_implementation->register_backend(std::move(backend));
}

void device_communicator_manager::enumerate_backends(std::vector<std::string> &backends) const
{
    m_implementation->enumerate_backends(backends);
}

device_communicator_backend* 
device_communicator_manager::get_backend(const std::string &name) const
{
    return m_implementation->get_backend(name);
}

device_communicator_backend* 
device_communicator_manager::find_compatible_backend(const device &dev) const
{
    return m_implementation->find_compatible_backend(dev);
}

std::unique_ptr<device_communicator>
device_communicator_manager::create_communicator(device &dev,
                                                 const std::shared_ptr<host_communicator> &comm) const
{
    std::unique_ptr<device_communicator> result;

    auto* backend = find_compatible_backend(dev);
    if (backend)
    {
        result = backend->create_communicator(dev, comm);
    }
    
    return result;
}

std::shared_ptr<device_communicator> 
device_communicator_manager::create_communicator_shared(device &dev,
                                                        const std::shared_ptr<host_communicator> &comm) const
{
    std::shared_ptr<device_communicator> result;

    auto* backend = find_compatible_backend(dev);
    if (backend)
    {
        result = backend->create_communicator_shared(dev, comm);
    }
    
    return result;
}

} // namespace system
} // namespace xmipp4
