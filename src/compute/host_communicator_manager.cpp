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
 * @file host_communicator_manager.cpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Implementation of host_communicator_manager.hpp
 * @date 2024-10-25
 * 
 */

#include <xmipp4/core/compute/host_communicator_manager.hpp>

#include <xmipp4/core/compute/host_communicator_backend.hpp>

#include <tuple>
#include <unordered_map>

namespace xmipp4
{
namespace compute
{

class host_communicator_manager::implementation
{
public:
    implementation() = default;
    ~implementation() = default;

    bool register_backend(std::unique_ptr<host_communicator_backend> backend)
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

        for (const auto &backend : m_registry)
        {
            backends.emplace_back(backend.first);
        }
    }

    host_communicator_backend* get_backend(const std::string &name) const
    {
        const auto ite = m_registry.find(name);

        host_communicator_backend *result = nullptr;
        if (ite != m_registry.end())
        {
            result = ite->second.get();
        }

        return result;
    }

    std::shared_ptr<host_communicator>
    get_world_communicator(const std::string &name) const
    {
        std::shared_ptr<host_communicator> result;
        
        const auto* backend = get_backend(name);
        if (backend)
        {
            result = backend->get_world_communicator();
        }

        return result;
    }

private:
    using registry_type = 
        std::unordered_map<std::string, std::unique_ptr<host_communicator_backend>>;

    registry_type m_registry;

};





host_communicator_manager::host_communicator_manager() = default;

host_communicator_manager::host_communicator_manager(host_communicator_manager&& other) noexcept = default;

host_communicator_manager::~host_communicator_manager() = default;

host_communicator_manager& 
host_communicator_manager::operator=(host_communicator_manager&& other) noexcept = default;



bool host_communicator_manager::register_backend(std::unique_ptr<host_communicator_backend> backend)
{
    return m_implementation->register_backend(std::move(backend));
}

void host_communicator_manager::enumerate_backends(std::vector<std::string> &backends) const
{
    m_implementation->enumerate_backends(backends);
}

host_communicator_backend* 
host_communicator_manager::get_backend(const std::string &name) const
{
    return m_implementation->get_backend(name);
}

std::shared_ptr<host_communicator>
host_communicator_manager::get_world_communicator(const std::string &name) const
{
    return m_implementation->get_world_communicator(name);
}

} // namespace system
} // namespace xmipp4
