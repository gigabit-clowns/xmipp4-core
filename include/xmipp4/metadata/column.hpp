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

#include "typed_column.hpp"

#include <memory>
#include <typeinfo>
#include <type_traits>

namespace xmipp4
{
namespace metadata
{

class column
{
public:
    column() = default;
    column(const column& other);
    column(column&& other) = default;
    ~column() = default;
    
    column& operator=(const column& other);
    column& operator=(column&& other) = default;

    void swap(column other) noexcept;

    const std::type_info& get_value_type() const noexcept;
    
    typed_column_base& get_typed_column_base();
    const typed_column_base& get_typed_column_base() const;
    template<typename T, typename = typename std::enable_if<!std::is_void<T>::value>::type>
    typed_column<T>& get_typed_column();
    template<typename T, typename = typename std::enable_if<!std::is_void<T>::value>::type>
    const typed_column<T>& get_typed_column() const;

    void resize(std::size_t size);
    std::size_t size() const noexcept;

    void reserve(std::size_t capacity);
    std::size_t capacity() const noexcept;

    template<typename T, typename = typename std::enable_if<!std::is_void<T>::value>::type>
    T* data();
    template<typename T, typename = typename std::enable_if<!std::is_void<T>::value>::type>
    const T* data() const;

    template<typename T, typename... Args>
    void emplace(Args&&... args);
    template<typename T>
    void emplace(std::initializer_list<T> init);
    template<typename T>
    void emplace(const std::vector<T>& v);
    template<typename T>
    void emplace(std::vector<T>&& v);
    template<typename ForwardIt>
    void emplace(ForwardIt first, ForwardIt last);

private:
    std::unique_ptr<typed_column_base> m_typed_column;

};

} // namespace metadata
} // namespace xmipp4

#include "column.inl"