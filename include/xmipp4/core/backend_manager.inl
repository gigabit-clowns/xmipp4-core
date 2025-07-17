// SPDX-License-Identifier: GPL-3.0-only

#pragma once

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
