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

#include <cstddef>

namespace xmipp4
{
namespace metadata
{

class table;

class table_writer
{
public:
    table_writer() = default;
    table_writer(const table_writer& other) = delete;
    table_writer(table_writer&& other) = default;
    virtual ~table_writer() = default;

    table_writer& operator=(const table_writer& other) = delete;
    table_writer& operator=(table_writer&& other) = default;

    virtual std::size_t write(const table& table) = 0;

    virtual std::size_t tell() const noexcept = 0;
};

} // namespace metadata
} // namespace xmipp4
