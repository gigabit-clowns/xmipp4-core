// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "service_catalog.hpp"

#include "platform/constexpr.hpp"
#include "platform/assert.hpp"

namespace xmipp4 
{

template <typename T>
inline
typename std::enable_if<std::is_convertible<T*, service_manager*>::value, T&>::type
service_catalog::get_service_manager()
{
    const std::type_index type(typeid(T));

    T* result = static_cast<T*>(get_service_manager(type));
    if(result == nullptr)
    {
        // Interface does not exist. Create it
        auto new_interface = std::make_unique<T>();
        result = new_interface.get();
        create_service_manager(type, std::move(new_interface));
    }

    return *result;
}

} // namespace xmipp4
