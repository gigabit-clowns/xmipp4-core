// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "interface_catalog.hpp"

#include "platform/constexpr.hpp"
#include "platform/assert.hpp"

namespace xmipp4 
{

template <typename T>
inline
typename std::enable_if<std::is_convertible<T*, backend_manager*>::value, T&>::type
interface_catalog::get_backend_manager()
{
    const std::type_index type(typeid(T));

    T* result = static_cast<T*>(get_backend_manager(type));
    if(result == nullptr)
    {
        // Interface does not exist. Create it
        auto new_interface = std::make_unique<T>();
        result = new_interface.get();
        create_backend_manager(type, std::move(new_interface));
    }

    return *result;
}

} // namespace xmipp4
