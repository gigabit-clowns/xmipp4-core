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
 * @file non_owning_storage.inl
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Implementation of non_owning_storage.hpp
 * @date 2025-03-14
 * 
 */

#include "non_owning_storage.hpp"

namespace xmipp4 
{
namespace multidimensional
{

template <typename T>
inline
non_owning_storage<T>::non_owning_storage(pointer data, 
                                          std::size_t count ) noexcept
    : m_data(data)
    , m_count(count)
{
}

template <typename T>
template <typename Q>
inline
non_owning_storage<T>::non_owning_storage(const non_owning_storage<Q> &other) noexcept
    : m_data(other.m_data)
    , m_count(other.m_count)
{
}

template <typename T>
inline
void non_owning_storage<T>::reset() noexcept
{
    m_data = nullptr;
    m_count = 0;
}

template <typename T>
inline
void non_owning_storage<T>::swap(non_owning_storage &other) noexcept
{
    std::swap(m_data, other.m_data);
    std::swap(m_count, other.m_count);
}

template <typename T>
inline
typename non_owning_storage<T>::view_type 
non_owning_storage<T>::view() noexcept
{
    return *this;
}

template <typename T>
inline
typename non_owning_storage<T>::const_view_type 
non_owning_storage<T>::view() const noexcept
{
    return *this;
}

template <typename T>
inline
typename non_owning_storage<T>::pointer non_owning_storage<T>::data() noexcept
{
    return m_data;
}

template <typename T>
inline
typename non_owning_storage<T>::const_pointer 
non_owning_storage<T>::data() const noexcept
{
    return m_data;
}

template <typename T>
inline
std::size_t non_owning_storage<T>::size() const noexcept
{
    return m_count;
}

template <typename T>
inline
void swap(non_owning_storage<T> &lhs, non_owning_storage<T> &rhs) noexcept
{
    lhs.swap(rhs);
}

} // namespace multidimensional
} // namespace xmipp4
