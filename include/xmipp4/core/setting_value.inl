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
 * @file setting_value.inl
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Implementation of setting_value.hpp
 * @date 2025-05-09
 * 
 */

#include "setting_value.hpp"

#include <cstdlib>

namespace xmipp4 
{

inline
setting_value::setting_value()
    : m_source(setting_source::unset)
{
}

template <typename Str>
inline
setting_value::setting_value(Str &&value, setting_source source)
    : m_source(source)
    , m_value(std::forward<Str>(value))
{
}

bool setting_value::is_set() const noexcept
{
    return m_source != setting_source::unset;
}

inline
setting_source setting_value::get_source() const noexcept
{
    return m_source;
}

inline
const std::string& setting_value::get_value() const noexcept
{
    return m_value;
}

template <typename Str>
inline
bool setting_value::update(Str &&value, setting_source source)
{
    bool result = false;

    if (source > m_source)
    {
        m_source = source;
        m_value = std::forward<Str>(value);
        result = true;
    }

    return result;
}

inline
bool setting_value::update(const setting_value &other)
{
    return update(other.get_value(), other.get_source());
}

inline
bool setting_value::update_from_environment_variable(const std::string &name)
{
    bool result = false;
    if (setting_source::environment_variable > m_source)
    {
        const char *env_value = std::getenv(name.c_str());
        if (env_value)
        {
            m_source = setting_source::environment_variable;
            m_value = std::string(env_value);
            result = true;
        }
    }
}

} // namespace xmipp4
