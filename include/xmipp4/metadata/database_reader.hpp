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

#include <memory>
#include <cstddef>

namespace xmipp4
{
namespace metadata
{

class table_reader;

class database_reader
{
public:
    database_reader() = default;
    database_reader(const database_reader& other) = delete;
    database_reader(database_reader&& other) = default;
    virtual ~database_reader() = default;

    database_reader& operator=(const database_reader& other) = delete;
    database_reader& operator=(database_reader&& other) = default;

    virtual std::unique_ptr<table_reader> create_table_reader(const std::string& label) = 0;
    virtual std::unique_ptr<table_reader> create_table_reader() = 0;

};

} // namespace metadata
} // namespace xmipp4

