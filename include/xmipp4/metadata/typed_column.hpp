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

#include "../platform/attributes.hpp"

#include <vector>
#include <typeinfo>

namespace xmipp4
{
namespace metadata
{

class allocation_context;

class typed_column_base
{
public:
    typed_column_base() = default;
    typed_column_base(const typed_column_base& other) = default;
    typed_column_base(typed_column_base&& other) = default;
    virtual ~typed_column_base() = default;

    typed_column_base& operator=(const typed_column_base& other) = default;
    typed_column_base& operator=(typed_column_base&& other) = default;

    virtual const std::type_info& get_value_type() const noexcept = 0;
    virtual typed_column_base* clone() const = 0;

    virtual void resize(std::size_t size) = 0;
    virtual std::size_t size() const noexcept = 0;

    virtual void reserve(std::size_t capacity) = 0;
    virtual std::size_t capacity() const noexcept = 0;


};

template <typename T>
class typed_column final
    : public typed_column_base
{
public:
    using value_type = T;
    using pointer = value_type*;
    using const_pointer = const value_type*;

    typed_column() = default;
    template<typename... Args>
    explicit typed_column(Args&&... args);
    typed_column(const typed_column& other) = default;
    typed_column(typed_column&& other) = default;
    virtual ~typed_column() = default;

    typed_column& operator=(const typed_column& other) = default;
    typed_column& operator=(typed_column&& other) = default;

    const std::type_info& get_value_type() const noexcept override;
    typed_column* clone() const override;

private:
    std::vector<T> m_data;

};

} // namespace metadata
} // namespace xmipp4

#include "typed_column.inl"
