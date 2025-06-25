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
 * @file storage_reference.inl
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Implementation of storage_reference.hpp
 * @date 2025-03-14
 * 
 */

#include "storage_reference.hpp"

namespace xmipp4 
{
namespace multidimensional
{

template <typename T>
template <typename... Args>
inline
storage_reference<T>::storage_reference(Args&&... args)
    : m_storage(std::make_shared<storage_type>(std::forward<Args>(args)...))
{
}

template <typename T>
inline
storage_reference<T>::storage_reference(const storage_reference<const storage_type> &other)
    : m_storage(other.m_storage)
{
}

template <typename T>
inline
storage_reference<T>::storage_reference(storage_reference<const storage_type> &&other)
    : m_storage(std::move(other.m_storage))
{
}

template <typename T>
inline
void storage_reference<T>::reset() noexcept
{
    m_storage.reset();
}

template <typename T>
inline
void storage_reference<T>::swap(storage_reference &other) noexcept
{
    m_storage.swap(other.m_storage);
}

template <typename T>
inline
bool storage_reference<T>::aliases(const storage_reference &other) const noexcept
{
    return m_storage == other.m_storage;
}

template <typename T>
inline
typename storage_reference<T>::storage_type* 
storage_reference<T>::get() noexcept
{
    return m_storage.get();
}

template <typename T>
inline
const typename storage_reference<T>::storage_type* 
storage_reference<T>::get() const noexcept
{
    return m_storage.get();
}

template <typename T>
inline
std::size_t storage_reference<T>::size() const noexcept
{
    return m_storage ? m_storage->size() : 0;
}



template <typename T>
inline 
void swap(storage_reference<T> &lhs, storage_reference<T> &rhs) noexcept
{
    lhs.swap(rhs);
}

} // namespace multidimensional
} // namespace xmipp4
