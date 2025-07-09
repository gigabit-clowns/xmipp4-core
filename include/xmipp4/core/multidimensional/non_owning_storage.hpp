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

/**
 * @file non_owning_storage.hpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Defines non_owning_storage class
 * @date 2025-03-14
 * 
 */

#include <cstddef>

namespace xmipp4 
{
namespace multidimensional
{

template <typename T>
class non_owning_storage
{
public:
    using value_type = T;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using is_host_accessible = std::true_type;
    using view_type = non_owning_host_storage<value_type>;
    using const_view_type = non_owning_host_storage<const value_type>;

    non_owning_storage() = default;
    non_owning_storage(pointer data, std::size_t count) noexcept;
    template <typename Q>
    non_owning_storage(const non_owning_storage<Q> &other) noexcept;
    non_owning_storage(const non_owning_storage &other) = default;
    non_owning_storage(non_owning_storage &&other) = default;
    ~non_owning_storage() = default;

    non_owning_storage& operator=(const non_owning_storage &other) = default;
    non_owning_storage& operator=(non_owning_storage &&other) = default;

    void reset() noexcept;
    void swap(non_owning_storage &other) noexcept;

    view_type view() noexcept;
    const_view_type view() const noexcept;

    pointer data() noexcept;
    const_pointer data() const noexcept;

    std::size_t size() const noexcept;

private:
    pointer m_data;
    std::size_t m_count;

};

template <typename T>
void swap(non_owning_storage<T> &lhs, non_owning_storage<T> &rhs) noexcept;

} // namespace multidimensional
} // namespace xmipp4

#include "non_owning_storage.inl"
