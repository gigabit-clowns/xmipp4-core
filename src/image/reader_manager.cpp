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
 * @file reader_manager.cpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Implementation of reader_manager.hpp
 * @date 2024-10-25
 * 
 */

#include <xmipp4/core/image/reader_manager.hpp>

#include <xmipp4/core/exceptions/ambiguous_backend_error.hpp>
#include <xmipp4/core/image/reader_backend.hpp>

#include <tuple>
#include <vector>
#include <unordered_map>
#include <algorithm>

namespace xmipp4
{
namespace image
{

class reader_manager::implementation
{
public:
    implementation() = default;
    ~implementation() = default;

    bool register_backend(std::unique_ptr<reader_backend> backend)
    {
        bool inserted = false;
        if (backend)
        {
            auto key = backend->get_name();
            std::tie(std::ignore, inserted) = m_catalog.emplace(
                std::move(key), std::move(backend)
            );
        }

        return inserted;
    }

    void enumerate_backends(std::vector<std::string> &backends) const
    {
        backends.clear();
        backends.reserve(m_catalog.size());

        for (const auto &backend : m_catalog)
        {
            backends.emplace_back(backend.first);
        }
    }

    reader_backend* get_backend(const std::string &name) const
    {
        const auto ite = m_catalog.find(name);

        reader_backend *result = nullptr;
        if (ite != m_catalog.end())
        {
            result = ite->second.get();
        }

        return result;
    }

    reader_backend* find_backend_for_file(std::string_view path) const
    {   
        reader_backend* result;

        // Filter available backends
        std::vector<reader_backend*> available_backends;
        for (auto &item : m_catalog)
        {   
            if (item.second->supports_file(path))
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
                [] (const reader_backend *lhs, 
                    const reader_backend *rhs )
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
                    "reader_backend-s. Ensure that only one has "
                    "been installed."
                );
            }

            result = available_backends[0];
        }

        return result;
    }

private:
    using catalog_type = 
        std::unordered_map<std::string, std::unique_ptr<reader_backend>>;

    catalog_type m_catalog;

};



static 
std::shared_ptr<reader> 
create_reader(const reader_backend* backend, std::string_view path)
{
    std::shared_ptr<reader> result;

    if (backend)
    {
        result = backend->create_reader(path);
    }

    return result;
}



reader_manager::reader_manager() = default;

reader_manager::reader_manager(reader_manager&& other) noexcept = default;

reader_manager::~reader_manager() = default;

reader_manager& 
reader_manager::operator=(reader_manager&& other) noexcept = default;



void reader_manager::register_builtin_backends()
{
}

bool reader_manager::register_backend(std::unique_ptr<reader_backend> backend)
{
    return m_implementation->register_backend(std::move(backend));
}

void reader_manager::enumerate_backends(std::vector<std::string> &backends) const
{
    m_implementation->enumerate_backends(backends);
}

reader_backend* reader_manager::get_backend(const std::string &name) const
{
    return m_implementation->get_backend(name);
}

std::shared_ptr<reader> create_reader(std::string_view path) const
{
    return nullptr; // TODO
}


} // namespace image
} // namespace xmipp4
