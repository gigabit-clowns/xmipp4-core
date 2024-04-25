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

#include "shared_storage.hpp"

namespace xmipp4 
{
namespace multidimensional
{

template <typename T>
inline
shared_storage<T>::shared_storage(std::size_t size)
{
    if (size) 
    {
        m_data = std::make_shared<container_type>(size);
    }
}

template <typename T>
template <typename Q>
inline
shared_storage<T>::shared_storage(const shared_storage<Q>& other)
    : m_data(other.m_data)
{
}

template <typename T>
template <typename Q>
inline
shared_storage<T>::shared_storage(shared_storage<Q>&& other)
    : m_data(std::move(other.m_data))
{
}



template <typename T>
inline
typename shared_storage<T>::alias_type 
shared_storage<T>::alias() noexcept
{
    return *this;
}

template <typename T>
inline
typename shared_storage<T>::const_alias_type 
shared_storage<T>::alias() const noexcept
{
    return *this;
}

template <typename T>
inline
typename shared_storage<T>::const_alias_type 
shared_storage<T>::view() const noexcept
{
    return alias();
}



template <typename T>
inline
std::size_t shared_storage<T>::size() const noexcept
{
    return m_data ? m_data->size() : 0UL;
}

template <typename T>
inline
void  shared_storage<T>::resize(std::size_t size)
{
    if (m_data.use_count() == 1)
    {
        // Only owned by this instance. Resize inplace.
        m_data->resize(size);
    }
    else
    {   
        // Container referenced by others. Cannot modify it
        if (size)
        {
            m_data = std::make_shared<container_type>(size);
        }
        else
        {   
            m_data.reset();
        }
    }
}

template <typename T>
inline
typename shared_storage<T>::value_type* 
shared_storage<T>::data() noexcept
{
    return m_data ? m_data->data() : nullptr;
}

template <typename T>
inline
const typename shared_storage<T>::value_type*
shared_storage<T>::data() const noexcept
{
    return m_data ? m_data->data() : nullptr;
}

} // namespace multidimensional
} // namespace xmipp4

