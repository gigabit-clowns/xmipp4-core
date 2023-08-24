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

#include "allocation_context.hpp"

#include <vector>
#include <memory>
#include <string>
#include <unordered_map>
#include <cstddef>

namespace xmipp4
{
namespace metadata
{

class column_base;

class table
{
public:
    table() = default;
    table(const table& other) = delete;
    table(table&& other) = default;
    ~table() = default;

    table& operator=(const table& other) = delete;
    table& operator=(table&& other) = default;

    void resize(std::size_t size);
    void reserve(std::size_t capacity);

    void add_column(std::string label, 
                    std::unique_ptr<column_base> column, 
                    const allocation_context& allocated = allocation_context() );

private:
    allocation_context m_allocated;
    std::vector<std::unique_ptr<column_base>> m_columns;
    std::unordered_map<std::string, std::size_t> m_label_to_index;

    void resize(const allocation_context& context);

};

} // namespace metadata
} // namespace xmipp4
