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

#include "label_map.hpp"
#include "column.hpp"

#include <vector>

namespace xmipp4
{
namespace metadata
{

class table
{
public:
    table() = default;
    table(const table& other) = delete;
    table(table&& other) = default;
    ~table() = default;

    table& operator=(const table& other) = delete;
    table& operator=(table&& other) = default;

    std::size_t rows() const noexcept;
    std::size_t columns() const noexcept;
    
    void clear() noexcept;

    void resize(std::size_t size);

    template<typename Label, typename Column>
    bool add_column(Label&& label, Column&& column);

private:
    std::size_t m_size;
    label_map<column> m_columns;

};

} // namespace metadata
} // namespace xmipp4

#include "table.inl"
