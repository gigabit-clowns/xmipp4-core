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
        m_typed_column.reset(other.m_typed_column->clone());
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

typed_column_base& column::get_typed_column_base()
{
    if(m_typed_column)
        return *m_typed_column;
    else
        throw std::runtime_error("No typed_column was assigned");
}

const typed_column_base& column::get_typed_column_base() const
{
    if(m_typed_column)
        return *m_typed_column;
    else
        throw std::runtime_error("No typed_column was assigned");
}

template<typename T, typename = typename std::enable_if<!std::is_void<T>::value>::type>
inline
typed_column<T>& column::get_typed_column()
{
    auto& base = get_typed_column_base();
    if(base.get_value_type() == typeid(T))
        return static_cast<typed_column<T>&>(base);
    else
        return std::runtime_error("typed_column's type does not match the requested type");
}

template<typename T, typename = typename std::enable_if<!std::is_void<T>::value>::type>
const typed_column<T>& column::get_typed_column() const
{
    auto& base = get_typed_column_base();
    if(base.get_value_type() == typeid(T))
        return static_cast<typed_column<T>&>(base);
    else
        return std::runtime_error("typed_column's type does not match the requested type");
}

void column::resize(std::size_t size)
{
    get_typed_column_base().resize(size);
}

std::size_t column::size() const noexcept
{
    if(m_typed_column)
        return m_typed_column->size();
    else
        return 0;
}

void column::reserve(std::size_t capacity)
{
    get_typed_column_base().reserve(capacity);
}

std::size_t column::capacity() const noexcept
{
    if(m_typed_column)
        return m_typed_column->capacity();
    else
        return 0;
}

template<typename T, typename = typename std::enable_if<!std::is_void<T>::value>::type>
T* column::data()
{
    return get_typed_column<T>().data();
}

template<typename T, typename = typename std::enable_if<!std::is_void<T>::value>::type>
const T* column::data() const
{
    return get_typed_column<T>().data();
}

template<typename T, typename... Args>
void column::emplace(Args&&... args)
{
    m_typed_column = std::make_unique<typed_column<T>>(std::forward<Args>(args)...);
}

template<typename T>
void column::emplace(std::initializer_list<T> init)
{
    emplace<T, std::initializer_list<T>>(init);
}

template<typename T>
void column::emplace(const std::vector<T>& v)
{
    emplace<T, const std::vector<T>&>(v);
}

template<typename T>
void column::emplace(std::vector<T>&& v)
{
    emplace<T, std::vector<T>&&>(std::move(v));
}

template<typename ForwardIt>
void column::emplace(ForwardIt first, ForwardIt last)
{
    using T = typename std::iterator_traits<ForwardIt>::value_type;
    emplace<T, ForwardIt, ForwardIt>(first, last);
}

} // namespace metadata
} // namespace xmipp4
