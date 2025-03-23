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
 * @file basic_storage.hpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Defines basic_storage class
 * @date 2025-03-14
 * 
 */

#include <vector>

namespace xmipp4 
{
namespace multidimensional
{

template <typename T>
class basic_storage
{
public:
    using value_type = T;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using is_host_accessible = std::true_type;
    using may_alias = std::false_type;

    basic_storage() = default;
    explicit basic_storage(std::size_t n);
    basic_storage(const basic_storage &other) = default;
    basic_storage(basic_storage &&other) = default;
    ~basic_storage() = default;

    basic_storage& operator=(const basic_storage &other) = default;
    basic_storage& operator=(basic_storage &&other) = default;

    void reset() noexcept;
    void swap(basic_storage &other) noexcept;

    pointer data() noexcept;
    const_pointer data() const noexcept;

    std::size_t size() const noexcept;

private:
    std::vector<value_type> m_data;

};

template <typename T>
void swap(basic_storage<T> &lhs, basic_storage<T> &rhs) noexcept;

} // namespace multidimensional
} // namespace xmipp4

#include "basic_storage.inl"
