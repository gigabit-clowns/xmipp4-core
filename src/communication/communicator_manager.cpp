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
#include <xmipp4/core/exceptions/ambiguous_backend_error.hpp>

#include <tuple>
#include <vector>
#include <unordered_map>
#include <algorithm>

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

        // Filter available backends
        std::vector<communicator_backend*> available_backends;
        available_backends.reserve(m_registry.size());
        for (auto &item : m_registry)
        {   
            if (item.second->is_available())
            {
                available_backends.emplace_back(item.second.get());
            }
        }

        if( available_backends.empty())
        {
            result = nullptr;
        }
        else if (available_backends.size() == 1)
        {
            result = available_backends[0];
        }
        else // available_backends.size() > 1
        {
            // Get the best two
            std::partial_sort(
                available_backends.begin(),
                std::next(available_backends.begin(), 2),
                available_backends.end(),
                [] (const communicator_backend *lhs, 
                    const communicator_backend *rhs )
                {
                    return lhs->get_priority() > rhs->get_priority();
                }
            );

            // Ensure the second is worse
            if (available_backends[0]->get_priority() == 
                available_backends[1]->get_priority() )
            {
                throw ambiguous_backend_error(
                    "Could not disambiguate among multiple "
                    "communicator_backend-s. Ensure that only one has "
                    "been installed."
                );
            }

            result = available_backends[0];
        }

        return result;
    }

private:
    using registry_type = 
        std::unordered_map<std::string, std::unique_ptr<communicator_backend>>;

    registry_type m_registry;

};



static 
std::shared_ptr<communicator> 
obtain_communicator(const communicator_backend* backend)
{
    std::shared_ptr<communicator> result;
    
    if (backend)
    {
        result = backend->get_world_communicator();
    }

    return result;
}



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
    return obtain_communicator(get_backend(name));
}

std::shared_ptr<communicator>
communicator_manager::get_preferred_world_communicator() const
{
    return obtain_communicator(get_preferred_backend());
}

} // namespace communication
} // namespace xmipp4
