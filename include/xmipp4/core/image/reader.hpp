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
 * @file reader.hpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Definition of the image::reader class
 * @date 2025-05-07
 * 
 */

#include <cstddef>
#include <vector>

namespace xmipp4 
{
namespace image
{

/**
 * @brief Abstract class to read images from a file.
 * 
 */
class reader
{
public:
    reader() = default;
    reader(const reader &) = default;
    reader(reader &&) = default;
    virtual ~reader() = default;
    
    reader &operator=(const reader &) = default;
    reader &operator=(reader &&) = default;

    // virtual numeric_type get_numerical_type() const = 0; // TODO
    virtual void get_extent(std::vector<std::size_t> &extent) const = 0;

    // TODO retrieve other information
    virtual void read() const = 0; // TODO return
    virtual void read_single(std::size_t position) const = 0; // TODO return
    virtual void read_batch(std::size_t first_position, std::size_t count) const = 0; // TODO return

};

} // namespace image
} // namespace xmipp4
