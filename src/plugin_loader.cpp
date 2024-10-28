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
 * @file plugin_loader.cpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Implementation of plugin_loader.hpp
 * @date 2024-03-11
 * 
 */

#include "plugin_loader.hpp"

#include <stdexcept>

#define XMIPP4_PLUGIN_HOOK_SYMBOL_NAME "xmipp4_get_plugin"

namespace xmipp4
{

static const plugin* query_plugin(const system::dynamic_library& lib)
{
    using get_plugin_function_type = const plugin* (*)();
    const char symbol_name[] = XMIPP4_PLUGIN_HOOK_SYMBOL_NAME;

    const auto func = reinterpret_cast<get_plugin_function_type>(
        lib.get_symbol(symbol_name)
    );
    
    if (!func)
    {
        throw std::runtime_error(
            XMIPP4_PLUGIN_HOOK_SYMBOL_NAME
            " symbol could not be found in shared object."
        );
    }

    const auto* result = func();
    if(!result)
    {
        throw std::runtime_error(
            XMIPP4_PLUGIN_HOOK_SYMBOL_NAME
            " returned NULL"
        );
    }

    return result;
}



plugin_loader::plugin_loader()
    : m_dynamic_library()
    , m_plugin(nullptr)
{
}

plugin_loader::plugin_loader(const std::string& path)
    : m_dynamic_library(path)
    , m_plugin(query_plugin(m_dynamic_library))
{
}

const plugin* plugin_loader::get_plugin() const noexcept
{
    return m_plugin;
}

bool plugin_loader::is_open() const noexcept
{
    return m_dynamic_library.is_open();
}

void plugin_loader::reset()
{
    *this = plugin_loader();
}

void plugin_loader::load(const std::string& path)
{
    *this = plugin_loader(path);
}

} // namespace xmipp4
