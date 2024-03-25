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

#include <xmipp4/plugin_manager.hpp>

#include <xmipp4/plugin.hpp>
#include <xmipp4/plugin_loader.hpp>

#include <vector>
#include <cstdlib>

#define XMIPP4_PLUGIN_PATH_ENV "XMIPP4_PLUGIN_PATH"
#define XMIPP4_DEFAULT_PLUGIN_SUBDIRECTORY "xmipp4-plugins"

namespace xmipp4
{

static const char* get_default_plugin_path() noexcept
{
    #if XMIPP4_CHECK_OS(LINUX)
        return "/usr/local/lib/" XMIPP4_DEFAULT_PLUGIN_SUBDIRECTORY;
    #elif XMIPP4_CHECK_OS(APPLE)

    #elif XMIPP4_CHECK_OS(WINDOWS)

    #else
        #error "Unkown platform"
    #endif
}

static const char* get_plugin_path() noexcept
{
    const char* result = std::getenv(XMIPP4_PLUGIN_PATH_ENV);
    if (result == nullptr)
    {
        result = get_default_plugin_path();
    }
    return result;
}

class plugin_manager::implementation
{
public:
    implementation() = default;

    void add_plugin(const plugin* plugin)
    {
        if (plugin)
        {
            m_plugins.push_back(plugin);
        }
    }

    void load_plugin(const std::string &path)
    {
        const auto ite = m_loaders.emplace(m_loaders.cend(), path);
        auto first = ite->begin();
        const auto last = ite->end();

        for (; first != last; ++first)
        {
            add_plugin(*first);
        }
    }


private:
    std::vector<const plugin*> m_plugins;
    std::vector<plugin_loader> m_loaders;

};

plugin_manager::plugin_manager() = default;

plugin_manager::plugin_manager(plugin_manager&& other) = default;

plugin_manager::~plugin_manager() = default;

plugin_manager& plugin_manager::operator=(plugin_manager&& other) = default;

} // namespace xmipp4
