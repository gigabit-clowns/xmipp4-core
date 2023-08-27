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

#include "column.hpp"

namespace xmipp4
{
namespace metadata
{

inline
column::column(const column& other)
    : m_typed_column()
{
    if(other.m_typed_column)
    {
        m_typed_column = other.m_typed_column->clone();
    }
}

inline
column& column::operator=(const column& other)
{
    *this = column(other);
}

inline
void column::swap(column other) noexcept
{
    m_typed_column.swap(other.m_typed_column);
}

inline
const std::type_info& column::get_value_type() const noexcept
{
    return m_typed_column ? m_typed_column->get_value_type() : typeid(void);
}

template<typename T>
T* column::get_data() noexcept
{
    T* data = nullptr;
    if(typeid(T) == get_value_type())
    {
        if (m_typed_column)
            data = m_typed_column->get_data();
    }
    else
    {
        throw 
    }

    return data;
}

template<typename T>
const T* column::get_data() const noexcept
{

}

} // namespace metadata
} // namespace xmipp4

