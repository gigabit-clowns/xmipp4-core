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
 * @file plugin_loader.hpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Defines plugin_loader class
 * @date 2024-03-11
 * 
 */

#include <xmipp4/core/system/dynamic_library.hpp>

namespace xmipp4
{

class plugin;

/**
 * @brief Helper class to load plugins from shared objects. 
 * 
 * RAII style to free resources when destroyed.
 * 
 */
class plugin_loader
{
public:
    plugin_loader();

    /**
     * @brief Load a new plugin.
     * 
     * @param path Path to the Shared Object
     */
    explicit plugin_loader(const std::string& path);
    
    plugin_loader(const plugin_loader& other) = delete;
    plugin_loader(plugin_loader&& other) = default;
    ~plugin_loader() = default;

    plugin_loader& operator=(const plugin_loader& other) = delete;
    plugin_loader& operator=(plugin_loader&& other) = default;

    /**
     * @brief Get the loaded plugin.
     * 
     * May return nullptr if is_open() is false.
     * 
     * @return const plugin* The loaded plugin.
     */
    const plugin* get_plugin() const noexcept;

    /**
     * @brief Check if the shared object is loaded.
     * 
     * @return true The shared object is loaded.
     * @return false The shared object is not loaded.
     */
    bool is_open() const noexcept;

    /**
     * @brief Unloads the plugin. 
     * 
     * This method should be used with care as it does not handle
     * deregister the plugin.
     * 
     */
    void reset();

    /**
     * @brief Loads a new plugin.
     * 
     * If a plugin is already loaded, it will unload it. Use it with caution.
     * 
     * @param path Path to the Shared Object to be loaded.
     */
    void load(const std::string& path);

private:
    system::dynamic_library m_dynamic_library;
    const plugin* m_plugin;

};

} // namespace xmipp4
