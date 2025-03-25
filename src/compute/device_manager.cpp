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
 * @file device_manager.cpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Implementation of device_manager.hpp
 * @date 2024-10-23
 * 
 */

#include <xmipp4/core/compute/device_manager.hpp>

#include <xmipp4/core/compute/device_backend.hpp>

#include <unordered_map>

namespace xmipp4
{
namespace compute
{

class device_manager::implementation
{
public:
    bool register_backend(std::unique_ptr<device_backend> backend)
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

    device_backend* get_backend(const std::string &name) const
    {
        const auto ite = m_registry.find(name);

        device_backend *result = nullptr;
        if (ite != m_registry.end())
        {
            result = ite->second.get();
        }

        return result;
    }

    void enumerate_devices(std::vector<device_index>& indices) const
    {
        indices.clear();
        std::vector<std::size_t> ids;
        for(const auto &backend : m_registry)
        {
            backend.second->enumerate_devices(ids);
            for(const auto &id : ids)
            {
                indices.emplace_back(backend.first, id);
            }
        }
    }

private:
    std::unordered_map<std::string, std::unique_ptr<device_backend>> m_registry;

};





device_manager::device_manager() = default;

device_manager::device_manager(device_manager &&other) noexcept = default;

device_manager::~device_manager() = default;

device_manager& 
device_manager::operator=(device_manager &&other) noexcept = default;



bool device_manager::register_backend(std::unique_ptr<device_backend> backend)
{
    return m_implementation->register_backend(std::move(backend));
}

void device_manager::enumerate_backends(std::vector<std::string> &backends) const
{
    m_implementation->enumerate_backends(backends);
}

device_backend* device_manager::get_backend(const std::string &name) const
{
    return m_implementation->get_backend(name);
}

void device_manager::enumerate_devices(std::vector<device_index> &indices) const
{
    m_implementation->enumerate_devices(indices);
}

bool device_manager::get_device_properties(const device_index &index, 
                                           device_properties &desc ) const
{
    const auto *backend = get_backend(index.get_backend_name());

    bool result = backend != nullptr;
    if (result)
    {
        result = backend->get_device_properties(index.get_device_id(), desc);
    }

    return result;
}

std::shared_ptr<device> 
device_manager::create_device(const device_index& index,
                              const device_create_parameters &params ) const
{
    std::shared_ptr<device> result;

    auto *backend = get_backend(index.get_backend_name());
    if (backend)
    {
        result = backend->create_device(index.get_device_id(), params);
    }

    return result;
}

} // namespace compute
} // namespace xmipp4
