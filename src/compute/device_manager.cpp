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

#include <xmipp4/core/compute/host/host_device_backend.hpp>

#include <unordered_map>

namespace xmipp4
{
namespace compute
{

void device_manager::register_builtin_backends()
{
    host_device_backend::register_at(*this);
}

void device_manager::enumerate_devices(std::vector<device_index> &indices) const
{
    std::vector<std::string> backend_names;
    enumerate_backends(backend_names);

    indices.clear();
    std::vector<std::size_t> ids;
    for(const auto &backend_name : backend_names)
    {
        get_backend(backend_name)->enumerate_devices(ids);
        for(const auto &id : ids)
        {
            indices.emplace_back(backend_name, id);
        }
    }
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
