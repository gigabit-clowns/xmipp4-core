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

#include <tuple>

namespace xmipp4
{
namespace compute
{

bool device_manager::register_backend(std::unique_ptr<device_backend> backend)
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

device_backend* device_manager::get_backend(const std::string &name) const
{
    const auto ite = m_registry.find(name);

    device_backend *result = nullptr;
    if (ite != m_registry.end())
    {
        result = ite->second.get();
    }

    return result;
}

std::vector<device_index> device_manager::enumerate_devices() const
{
    std::vector<device_index> result;
    enumerate_devices(result);
    return result;
}

void device_manager::enumerate_devices(std::vector<device_index>& indices) const
{
    indices.clear();
    std::vector<std::size_t> ids;
    for(auto ite = m_registry.cbegin(); ite != m_registry.cend(); ++ite)
    {
        const auto &backend_name = ite->first;
        const auto &backend = ite->second;

        backend->enumerate_devices(ids);
        for(const auto &id : ids)
        {
            indices.emplace_back(backend_name, id);
        }
    }
}

bool device_manager::get_device_descriptor(const device_index& index, 
                                           device_descriptor &desc ) const
{
    auto *backend = get_backend(index.get_backend_name());

    bool result = backend != nullptr;
    if (result)
    {
        result = backend->get_device_descriptor(index.get_device_id(), desc);
    }

    return result;
}

std::unique_ptr<device>
device_manager::create_device(const device_index& index) const
{
    auto *backend = get_backend(index.get_backend_name());
    if (backend)
    {
        return backend->create_device(index.get_device_id());
    }
    else
    {
        return nullptr;
    }
}

std::shared_ptr<device> 
device_manager::create_device_shared(const device_index& index) const
{
    auto *backend = get_backend(index.get_backend_name());
    if (backend)
    {
        return backend->create_device_shared(index.get_device_id());
    }
    else
    {
        return nullptr;
    }
}

} // namespace system
} // namespace xmipp4
