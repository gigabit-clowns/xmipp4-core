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

#include "table.hpp"

namespace xmipp4
{
namespace metadata
{

inline
std::size_t table::rows() const noexcept
{
    return m_size;
}

inline
std::size_t table::columns() const noexcept
{
    return m_columns.size();
}

inline
void table::clear() noexcept
{
    m_size = 0;
    m_columns.clear();
}

inline
void table::resize(std::size_t size)
{
    //TODO
}


} // namespace metadata
} // namespace xmipp4

