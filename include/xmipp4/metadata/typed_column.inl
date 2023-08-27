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

namespace xmipp4
{
namespace metadata
{


template <typename T>
inline
void typed_column<T>::resize(const allocation_context& prev, const allocation_context& next)
{
    //TODO
}

template <typename T>
inline
const std::type_info& typed_column<T>::get_value_type() const noexcept
{
    return typeid(value_type);
}

template <typename T>
inline
typename typed_column<T>::pointer typed_column<T>::get_data() noexcept
{
    //TODO
}

template <typename T>
inline
typename typed_column<T>::const_pointer typed_column<T>::get_data() const noexcept
{
    //TODO
}

template <typename T>
inline
typed_column<T>* typed_column<T>::clone() const
{
    return new typed_column(*this);
}



} // namespace metadata
} // namespace xmipp4

