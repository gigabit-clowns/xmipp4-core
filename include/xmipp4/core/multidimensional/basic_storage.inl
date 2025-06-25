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
 * @file basic_storage.inl
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Implementation of basic_storage.hpp
 * @date 2025-03-14
 * 
 */

#include "basic_storage.hpp"

namespace xmipp4 
{
namespace multidimensional
{

template <typename T>
inline
basic_storage<T>::basic_storage(std::size_t n)
    : m_data(n)
{
}

template <typename T>
inline
void basic_storage<T>::reset() noexcept
{
    m_data.clear();
}

template <typename T>
inline
void basic_storage<T>::swap(basic_storage &other) noexcept
{
    m_data.swap(other.m_data);
}

template <typename T>
inline
bool basic_storage<T>::aliases(const basic_storage &other) const noexcept
{
    return &other == this;
}

template <typename T>
inline
typename basic_storage<T>::pointer basic_storage<T>::data() noexcept
{
    return m_data.data();
}

template <typename T>
inline
typename basic_storage<T>::const_pointer basic_storage<T>::data() const noexcept
{
    return m_data.data();
}

template <typename T>
inline
std::size_t basic_storage<T>::size() const noexcept
{
    return m_data.size();
}

template <typename T>
inline
void swap(basic_storage<T> &lhs, basic_storage<T> &rhs) noexcept
{
    lhs.swap(rhs);
}

} // namespace multidimensional
} // namespace xmipp4
