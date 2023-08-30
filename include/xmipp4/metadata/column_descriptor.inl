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

#include "column_descriptor.hpp"

namespace xmipp4
{
namespace metadata
{

inline
void column_descriptor::set_label(const std::string& label)
{
    m_label = label;
}

inline
void column_descriptor::set_label(std::string&& label) noexcept
{
    m_label = std::move(label);
}

inline
const std::string& column_descriptor::get_label() const noexcept
{
    return m_label;
}

inline
void column_descriptor::set_category(const std::string& category)
{
    m_category = category;
}

inline
void column_descriptor::set_category(std::string&& category) noexcept
{
    m_category = std::move(category);
}

inline
const std::string& column_descriptor::get_category() const noexcept
{
    return m_category;
}

inline
void column_descriptor::set_type(std::type_index type) noexcept
{
    m_type = type;
}

inline
std::type_index column_descriptor::get_type() const noexcept
{
    return m_type;
}

} // namespace metadata
} // namespace xmipp4

