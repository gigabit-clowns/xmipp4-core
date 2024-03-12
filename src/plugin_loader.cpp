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

#include <xmipp4/plugin_loader.hpp>

#include <xmipp4/plugin.hpp>
#include <xmipp4/system/dynamic_library.hpp>

#include <stdexcept>

namespace xmipp4
{

class plugin_loader::implementation
{
public:
    implementation(const std::string& name)
        : m_dynamic_library(system::dynamic_library::make_soname(name))
        , m_count()
        , m_data(get_plugins(m_dynamic_library, m_count))
    {
    }

    const plugin* begin() const noexcept
    {
        return m_data;
    }

    const plugin* end() const noexcept
    {
        return m_data + m_count;
    }

private:

    system::dynamic_library m_dynamic_library;
    std::size_t m_count;
    const plugin* m_data;

    static const plugin* get_plugins(const system::dynamic_library& lib, 
                                     std::size_t &count )
    {
        using get_plugins_function_type = const plugin* (*)(std::size_t*);
        const char symbol_name[] = "xmipp4_get_plugins";

        const auto func = reinterpret_cast<get_plugins_function_type>(
            lib.get_symbol(symbol_name)
        );
        
        if (!func)
        {
            throw std::runtime_error("plugin hook could not be found");
        }

        return func(&count);
    }

};

plugin_loader::plugin_loader()
    : m_implementation(memory::defer_construct)
{
}

plugin_loader::plugin_loader(const std::string& name)
    : m_implementation(name)
{
}

plugin_loader::plugin_loader(plugin_loader&& other) = default;

plugin_loader::~plugin_loader() = default;

plugin_loader& plugin_loader::operator=(plugin_loader&& other) = default;

const plugin* plugin_loader::begin() const noexcept
{
    return m_implementation ? m_implementation->begin() : nullptr;
}

const plugin* plugin_loader::end() const noexcept
{
    return m_implementation ? m_implementation->end() : nullptr;
}

void plugin_loader::reset()
{
    m_implementation.reset();
}

}
