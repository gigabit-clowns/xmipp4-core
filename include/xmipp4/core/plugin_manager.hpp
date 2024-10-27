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

/**
 * @file plugin_manager.hpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Defines plugin_manager class
 * @date 2024-03-11
 * 
 */

#include "memory/pimpl.hpp"
#include "platform/dynamic_shared_object.h"

namespace xmipp4
{

class plugin;
class interface_registry;

/**
 * @brief Class managing a set of plugins.
 * 
 */
class XMIPP4_CORE_API plugin_manager
{
public:
    plugin_manager();
    plugin_manager(const std::string& name);
    plugin_manager(const plugin_manager& other) = delete;
    plugin_manager(plugin_manager&& other);
    ~plugin_manager();

    plugin_manager& operator=(const plugin_manager& other) = delete;
    plugin_manager& operator=(plugin_manager&& other);

    /**
     * @brief Add a already loaded plugin.
     * 
     * @param plugin The plugin to be added.
     */
    void add_plugin(const plugin& plugin);

    /**
     * @brief Load a plugin from a Shared Object and add it.
     * 
     * The path to the shared object can be provided without a extension,
     * in which case it will be inferred from the platform. Relative paths
     * will be searched in LD_LIBRARY_PATH.
     * 
     * @param path Path to the plugin. 
     * @return const plugin* The loaded plugin. nullptr if it could not be 
     * loaded.
     */
    const plugin* load_plugin(const std::string &path);

    /**
     * @brief Get the number of plugins known by this manager.
     * 
     * @return std::size_t The number of plugins.
     */
    std::size_t get_plugin_count() const noexcept;

    /**
     * @brief Get the i-th plugin.
     * 
     * @param index The index of the requested plugin. Must be less than
     * the plugin count.
     * @return const plugin& The requested plugin.
     */
    const plugin& get_plugin(std::size_t index) const;

private:
    class implementation;
    memory::pimpl<implementation> m_implementation;

};



/**
 * @brief Get the default plugin directory.
 * 
 * By default, plugins are searched in a directory named as "plugins" located
 * in the same directory as the shared library of the core.
 * 
 * @return std::string Default plugin directory.
 * 
 */
XMIPP4_CORE_API 
std::string get_default_plugin_directory();

/**
 * @brief Discover and load all plugins in a directory.
 * 
 * @param directory Directory where plugins are searched.
 * @param manager Plugin manager where plugins are loaded.
 * 
 */
XMIPP4_CORE_API
void discover_plugins(const std::string& directory, plugin_manager &manager);

/**
 * @brief Discover and load all plugins in the default directory.
 * 
 * @param manager Plugin manager where plugins are loaded.
 * 
 */
XMIPP4_CORE_API
void discover_plugins(plugin_manager &manager);

/**
 * @brief Register all plugins from a plugin manager into an
 * interface registry.
 * 
 * @param manager The plugin_manager that contains the plugins to
 * be registered.
 * @param registry Interface registry where plugins will be registered.
 * @return std::size_t Number of plugins registered.
 */
XMIPP4_CORE_API
std::size_t register_all_plugins_at(const plugin_manager &manager, 
                                    interface_registry &registry );

} // namespace xmipp4
