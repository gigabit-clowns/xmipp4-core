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
 * @date 2024-10-23
 * 
 */

#include <xmipp4/core/compute/device_communicator_manager.hpp>

#include <xmipp4/core/compute/device_communicator_backend.hpp>
#include <xmipp4/core/exceptions/ambiguous_backend_error.hpp>

#include <map>
#include <vector>
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
    find_supported_backend(span<device*> devices) const
    {   
        std::vector<backend*> available_backends;
        for (auto &item : m_registry)
        {   
            if (item.second->supports_devices(devices))
            {
                available_backends.emplace_back(item.second.get());
            }
        }

        return static_cast<device_communicator_backend*>(
            get_highest_priority_backend(make_span(available_backends))
        );
    }

private:
    using registry_type = 
        std::map<std::string, std::unique_ptr<device_communicator_backend>>;
    
    registry_type m_registry;

};





device_communicator_manager::device_communicator_manager() = default;

device_communicator_manager::device_communicator_manager(device_communicator_manager &&other) noexcept = default;

device_communicator_manager::~device_communicator_manager() = default;

device_communicator_manager& 
device_communicator_manager::operator=(device_communicator_manager &&other) noexcept = default;



bool device_communicator_manager::register_backend(std::unique_ptr<device_communicator_backend> backend)
{
    return m_implementation->register_backend(std::move(backend));
}

device_communicator_backend* 
device_communicator_manager::get_backend(const std::string &name) const
{
    return m_implementation->get_backend(name);
}

device_communicator_backend* 
device_communicator_manager::find_supported_backend(span<device*> devices) const
{
    return m_implementation->find_supported_backend(devices);
}

bool device_communicator_manager::create_device_communicators(node_communicator &node_communicator,
                                                              span<device*> devices,
                                                              span<std::unique_ptr<device_communicator>> &result ) const
{
    const auto* backend = find_supported_backend(devices);
    if(backend)
    {
        backend->create_device_communicators(
            node_communicator, 
            devices, 
            result
        );
    }
    return backend;
}

bool device_communicator_manager::create_device_communicators_shared(node_communicator &node_communicator,
                                                                     span<device*> devices,
                                                                     span<std::shared_ptr<device_communicator>> &result ) const
{
    const auto* backend = find_supported_backend(devices);
    if(backend)
    {
        backend->create_device_communicators_shared(
            node_communicator, 
            devices, 
            result
        );
    }
    return backend;
}

} // namespace compute
} // namespace xmipp4
