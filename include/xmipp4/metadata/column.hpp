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

#include "column_base.hpp"

#include <memory>
#include <vector>

namespace xmipp4
{
namespace metadata
{

template<typename T, typename Alloc = std::allocator<T>>
class column
    : public column_base
{
public:
    using value_type = T;
    using allocator_type = Alloc;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = typename std::allocator_traits<allocator_type>::pointer;
    using const_pointer = typename std::allocator_traits<allocator_type>::const_pointer;

    column() = default;
    template<typename... Args>
    explicit column(Args&&... args);
    column(const allocation_context& alloc) = default;
    column(const column& other) = default;
    column(column&& other) = default;
    virtual ~column() = default;

    column& operator=(const column& other) = default;
    column& operator=(column&& other) = default;

    void swap(column_base& other) noexcept;

    void resize(const allocation_context& prev, const allocation_context& next) final;
    pointer data() noexcept final;
    const_pointer data() const noexcept final;
private:
    std::vector<value_type> m_data;

};

} // namespace metadata
} // namespace xmipp4

#include "column.inl"
