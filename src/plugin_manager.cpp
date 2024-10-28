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
 * @file plugin_manager.cpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Implementation of plugin.hpp
 * @date 2024-03-11
 * 
 */

#include <xmipp4/core/plugin_manager.hpp>

#include "plugin_loader.hpp"

#include <xmipp4/core/plugin.hpp>

#include <vector>
#include <functional>
#include <cstdlib>

namespace xmipp4
{

class plugin_manager::implementation
{
public:
    implementation() = default;

    void add_plugin(const plugin& plugin)
    {
        m_plugins.emplace_back(plugin);
    }

    const plugin* load_plugin(const std::string &path)
    {
        const auto ite = m_loaders.emplace(m_loaders.cend(), path);
        const auto* plugin = ite->get_plugin();
        if (plugin)
        {
            add_plugin(*plugin);
        }
        else
        {
            m_loaders.erase(ite); // Did not load anything
        }

        return plugin;
    }

    std::size_t get_plugin_count() const noexcept
    {
        return m_plugins.size();
    }

    const plugin& get_plugin(std::size_t index) const
    {
        return m_plugins.at(index);
    }

private:
    std::vector<plugin_loader> m_loaders;
    std::vector<std::reference_wrapper<const plugin>> m_plugins;

};

plugin_manager::plugin_manager() = default;

plugin_manager::plugin_manager(plugin_manager&& other) noexcept = default;

plugin_manager::~plugin_manager() = default;

plugin_manager&
plugin_manager::operator=(plugin_manager&& other) noexcept = default;



void plugin_manager::add_plugin(const plugin& plugin)
{
    m_implementation->add_plugin(plugin);
}

const plugin* plugin_manager::load_plugin(const std::string &path)
{
    return m_implementation->load_plugin(path);
}

std::size_t plugin_manager::get_plugin_count() const noexcept
{
    return m_implementation->get_plugin_count();
}

const plugin& plugin_manager::get_plugin(std::size_t index) const
{
    return m_implementation->get_plugin(index);
}



std::size_t register_all_plugins_at(const plugin_manager &manager, 
                                    interface_registry &registry )
{
    const auto count = manager.get_plugin_count();
    for (std::size_t i = 0; i < count; ++i)
    {
        const auto &plugin = manager.get_plugin(i);
        plugin.register_at(registry);
    }
    return count;
}

} // namespace xmipp4
