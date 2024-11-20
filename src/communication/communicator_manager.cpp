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
 * @file communicator_manager.cpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Implementation of communicator_manager.hpp
 * @date 2024-10-25
 * 
 */

#include <xmipp4/core/communication/communicator_manager.hpp>

#include <xmipp4/core/communication/communicator_backend.hpp>

#include <tuple>
#include <unordered_map>

namespace xmipp4
{
namespace communication
{

class communicator_manager::implementation
{
public:
    implementation() = default;
    ~implementation() = default;

    bool register_backend(std::unique_ptr<communicator_backend> backend)
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

    communicator_backend* get_backend(const std::string &name) const
    {
        const auto ite = m_registry.find(name);

        communicator_backend *result = nullptr;
        if (ite != m_registry.end())
        {
            result = ite->second.get();
        }

        return result;
    }

    communicator_backend* get_preferred_backend() const
    {
        communicator_backend* result;

        // Find the first available backend
        auto ite = m_registry.begin();
        while(ite != m_registry.end() && !ite->second->is_available())
        {
            ++ite;
        }

        // Get the available backend with the highest priority
        if (ite == m_registry.end())
        {
            result = nullptr;
        }
        else
        {
            result = ite->second.get();
            auto highest_priority = result->get_priority();
            ++ite;

            while (ite != m_registry.end())
            {
                if (ite->second->is_available() && 
                    ite->second->get_priority() > highest_priority)
                {
                    result = ite->second.get();
                    highest_priority = result->get_priority();
                }

                ++ite;
            }
        }

        return result;
    }

private:
    using registry_type = 
        std::unordered_map<std::string, std::unique_ptr<communicator_backend>>;

    registry_type m_registry;

};





communicator_manager::communicator_manager() = default;

communicator_manager::communicator_manager(communicator_manager&& other) noexcept = default;

communicator_manager::~communicator_manager() = default;

communicator_manager& 
communicator_manager::operator=(communicator_manager&& other) noexcept = default;



bool communicator_manager::register_backend(std::unique_ptr<communicator_backend> backend)
{
    return m_implementation->register_backend(std::move(backend));
}

void communicator_manager::enumerate_backends(std::vector<std::string> &backends) const
{
    m_implementation->enumerate_backends(backends);
}

communicator_backend* 
communicator_manager::get_backend(const std::string &name) const
{
    return m_implementation->get_backend(name);
}

communicator_backend* 
communicator_manager::get_preferred_backend() const
{
    return m_implementation->get_preferred_backend();
}

std::shared_ptr<communicator>
communicator_manager::get_world_communicator(const std::string &name) const
{
    std::shared_ptr<communicator> result;
    
    const auto* backend = get_backend(name);
    if (backend)
    {
        result = backend->get_world_communicator();
    }

    return result;
}

std::shared_ptr<communicator>
communicator_manager::get_world_communicator() const
{
    std::shared_ptr<communicator> result;
    
    const auto* backend = get_preferred_backend();
    if (backend)
    {
        result = backend->get_world_communicator();
    }

    return result;
}

} // namespace communication
} // namespace xmipp4
