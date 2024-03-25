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
    explicit implementation(const std::string& path)
        : m_dynamic_library(path)
    {
        m_data = get_plugins(m_dynamic_library, m_count);
    }

    std::size_t count() const noexcept
    {
        return m_count;
    }

    const plugin* const* begin() const noexcept
    {
        return m_data;
    }

    const plugin* const* end() const noexcept
    {
        return begin() + count();
    }

private:
    system::dynamic_library m_dynamic_library;
    const plugin* const* m_data;
    std::size_t m_count;

    static const plugin* const* get_plugins(const system::dynamic_library& lib,
                                            std::size_t& count )
    {
        using get_plugins_function_type = const plugin* const* (*)(std::size_t*);
        const char symbol_name[] = "xmipp4_get_plugins";

        const auto func = reinterpret_cast<get_plugins_function_type>(
            lib.get_symbol(symbol_name)
        );
        
        if (!func)
        {
            throw std::runtime_error(
                "xmipp4_get_plugins symbol could not be found"
            );
        }

        return func(&count);
    }

};



plugin_loader::plugin_loader()
    : m_implementation(memory::defer_construct)
{
}

plugin_loader::plugin_loader(const std::string& path)
    : m_implementation(path)
{
}

plugin_loader::plugin_loader(plugin_loader&& other) = default;

plugin_loader::~plugin_loader() = default;

plugin_loader& plugin_loader::operator=(plugin_loader&& other) = default;



const plugin* const* plugin_loader::begin() const noexcept
{
    return m_implementation ? m_implementation->begin() : nullptr;
}

const plugin* const* plugin_loader::end() const noexcept
{
    return m_implementation ? m_implementation->end() : nullptr;
}

std::size_t plugin_loader::count() const noexcept
{
    return m_implementation ? m_implementation->count() : 0;
}

bool plugin_loader::is_open() const noexcept
{
    return static_cast<bool>(m_implementation);
}

void plugin_loader::reset()
{
    m_implementation.reset();
}

void plugin_loader::load(const std::string& path)
{
    m_implementation.emplace(path);
}

}
