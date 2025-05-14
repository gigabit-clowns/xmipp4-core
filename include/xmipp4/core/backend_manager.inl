#pragma once

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

#include "backend_manager.hpp"

namespace xmipp4 
{

template <typename B>
inline
bool basic_backend_manager<B>::register_backend(std::unique_ptr<backend_type> backend)
{
    bool inserted = false;
    if (backend)
    {
        auto key = backend->get_name();
        std::tie(std::ignore, inserted) = m_backends.emplace(
            std::move(key), std::move(backend)
        );
    }

    return inserted;
}

template <typename B>
inline
void basic_backend_manager<B>::enumerate_backends(std::vector<std::string> &backends) const
{
    backends.clear();
    backends.reserve(m_backends.size());
    for (const auto &backend : m_backends)
    {
        backends.emplace_back(backend.first);
    }
}

template <typename B>
inline
typename basic_backend_manager<B>::backend_type* 
basic_backend_manager<B>::get_backend(const std::string &name) const
{
    backend_type *result = nullptr;
    
    const auto ite = m_backends.find(name);
    if (ite != m_backends.end())
    {
        result = ite->second.get();
    }

    return result;
}

template <typename B>
inline
void basic_backend_manager<B>::get_backends(std::vector<backend_type*> &backends) const
{
    backends.clear();
    backends.reserve(m_backends.size());
    for (const auto& backend : m_backends)
    {
        backends.push_back(backend.second.get());
    }
}

template <typename B>
inline
void basic_backend_manager<B>::get_available_backends(std::vector<backend_type*> &backends) const
{
    backends.clear();
    backends.reserve(m_backends.size());
    for (const auto& backend : m_backends)
    {
        if (backend.second->is_available())
        {
            backends.push_back(backend.second.get());
        }
    }
}

} // namespace xmipp4
