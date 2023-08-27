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

#include <memory>
#include <typeinfo>

namespace xmipp4
{
namespace metadata
{

class typed_column_base;
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

    bool has_value() const noexcept;
    const std::type_info& get_value_type() const noexcept;
    
    template<typename T>
    T* get_data() noexcept;
    template<typename T>
    const T* get_data() const noexcept;

private:
    std::unique_ptr<typed_column_base> m_typed_column;

};

} // namespace metadata
} // namespace xmipp4

#include "coluimn.inl"