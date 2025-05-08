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
 * @file interface_catalog.cpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Implementation of plugin.hpp
 * @date 2024-03-11
 * 
 */

#include <xmipp4/core/interface_catalog.hpp>

#include "plugin_loader.hpp"

#include <xmipp4/core/plugin.hpp>

#include <vector>
#include <cstdlib>
#include <unordered_map>

namespace xmipp4
{

class interface_catalog::implementation
{
public:
    explicit implementation(bool register_builtin_backends)
        : m_register_builtin_backends(register_builtin_backends)
    {
    }

    ~implementation() = default;

    backend_manager* get_backend_manager(std::type_index type)
    {
        backend_manager* result = nullptr;

        const auto ite = m_interfaces.find(type);
        if(ite != m_interfaces.end())
        {
            result = ite->second.get();
        }

        return result;
    }

    void create_backend_manager(std::type_index type,
                                  std::unique_ptr<backend_manager> manager )
    {
        if (m_register_builtin_backends)
        {
            manager->register_builtin_backends();
        }

        m_interfaces.emplace(
            type, std::move(manager)
        );
    }

private:
    using catalog_type = 
        std::unordered_map<std::type_index, std::unique_ptr<backend_manager>>;

    bool m_register_builtin_backends;
    catalog_type m_interfaces;

};

interface_catalog::interface_catalog(bool register_builtin_backends)
    : m_implementation(register_builtin_backends)
{
}

interface_catalog::interface_catalog(interface_catalog&& other) noexcept = default;

interface_catalog::~interface_catalog() = default;

interface_catalog& 
interface_catalog::operator=(interface_catalog&& other) noexcept = default;


backend_manager* 
interface_catalog::get_backend_manager(std::type_index type)
{
    return m_implementation->get_backend_manager(type);
}
    
void interface_catalog::create_backend_manager(std::type_index type,
                                                  std::unique_ptr<backend_manager> manager )
{
    m_implementation->create_backend_manager(type, std::move(manager));
}

} // namespace xmipp4
