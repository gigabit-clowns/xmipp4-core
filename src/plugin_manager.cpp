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

#include <xmipp4/core/plugin.hpp>
#include <xmipp4/core/plugin_loader.hpp>

#include <vector>
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
        m_loaders.emplace_back(path);
        const auto* plugin = m_loaders.back().get_plugin();
        if (plugin)
        {
            add_plugin(*plugin);
        }
        else
        {
            m_loaders.pop_back(); // Did not load anything
        }

        return plugin;
    }

    std::size_t count() const noexcept
    {
        return m_plugins.size();
    }

    const plugin& get_plugin(std::size_t index) const
    {
        return m_plugins.at(index);
    }

private:
    std::vector<std::reference_wrapper<const plugin>> m_plugins;
    std::vector<plugin_loader> m_loaders;

};

plugin_manager::plugin_manager() = default;

plugin_manager::plugin_manager(plugin_manager&& other) = default;

plugin_manager::~plugin_manager() = default;

plugin_manager& plugin_manager::operator=(plugin_manager&& other) = default;



void plugin_manager::add_plugin(const plugin& plugin)
{
    m_implementation->add_plugin(plugin);
}

const plugin* plugin_manager::load_plugin(const std::string &path)
{
    return m_implementation->load_plugin(path);
}

std::size_t plugin_manager::count() const noexcept
{
    return m_implementation->count();
}

const plugin& plugin_manager::get_plugin(std::size_t index) const
{
    return m_implementation->get_plugin(index);
}

} // namespace xmipp4
