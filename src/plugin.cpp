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
 * @file plugin.cpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Implementation of plugin.hpp
 * @date 2024-03-11
 * 
 */

#include <xmipp4/plugin.hpp>

namespace xmipp4
{

class plugin::implementation
{
public:
    implementation(const std::string& name, version version)
        : m_name(name)
        , m_version(version)
    {
    }

    void set_name(const std::string& name)
    {
        m_name = name;
    }

    const std::string& get_name() const noexcept
    {
        return m_name;
    }

    void set_version(version version) noexcept
    {
        m_version = version;
    }

    version get_version() const noexcept
    {
        return m_version;
    }

private:
    std::string m_name;
    version m_version;

};

plugin::plugin(const plugin& other) = default;

plugin::plugin(plugin&& other) = default;

plugin::~plugin() = default;

plugin& plugin::operator=(const plugin& other) = default;

plugin& plugin::operator=(plugin&& other) = default;



void plugin::set_name(const std::string& name)
{
    m_implementation->set_name(name);
}

const std::string& plugin::get_name() const noexcept
{
    return m_implementation->get_name();
}

void plugin::set_version(version version) noexcept
{
    m_implementation->set_version(version);
}

version plugin::get_version() const noexcept
{
    return m_implementation->get_version();
}

} // namespace xmipp4