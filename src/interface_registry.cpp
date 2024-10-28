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
 * @file interface_registry.cpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Implementation of plugin.hpp
 * @date 2024-03-11
 * 
 */

#include <xmipp4/core/interface_registry.hpp>

#include "plugin_loader.hpp"

#include <xmipp4/core/plugin.hpp>

#include <vector>
#include <cstdlib>
#include <unordered_map>

namespace xmipp4
{

class interface_registry::implementation
{
public:
    implementation() = default;
    ~implementation() = default;

    interface_manager* get_interface_manager(std::type_index type)
    {
        interface_manager* result = nullptr;

        const auto ite = m_interfaces.find(type);
        if(ite != m_interfaces.end())
        {
            result = ite->second.get();
        }

        return result;
    }

    void create_interface_manager(std::type_index type,
                                  std::unique_ptr<interface_manager> manager )
    {
        m_interfaces.emplace(
            type, std::move(manager)
        );
    }

private:
    using registry_type = 
        std::unordered_map<std::type_index, std::unique_ptr<interface_manager>>;

    registry_type m_interfaces;

};

interface_registry::interface_registry() = default;

interface_registry::interface_registry(interface_registry&& other) noexcept = default;

interface_registry::~interface_registry() = default;

interface_registry& 
interface_registry::operator=(interface_registry&& other) noexcept = default;


interface_manager* 
interface_registry::get_interface_manager(std::type_index type)
{
    return m_implementation->get_interface_manager(type);
}
    
void interface_registry::create_interface_manager(std::type_index type,
                                                  std::unique_ptr<interface_manager> manager )
{
    m_implementation->create_interface_manager(type, std::move(manager));
}

} // namespace xmipp4
