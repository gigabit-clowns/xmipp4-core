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
 * @file reader_backend.hpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Definition of the image::reader_backend class
 * @date 2025-05-07
 * 
 */

#include "../backend.hpp"

#include <string_view>
#include <memory>
#include <cstddef>

namespace xmipp4 
{
namespace image
{

class reader;

/**
 * @brief Abstract backend class to create image readers. 
 * 
 */
class reader_backend
    : public backend
{
public:
    reader_backend() = default;
    reader_backend(const reader_backend &) = default;
    reader_backend(reader_backend &&) = default;
    virtual ~reader_backend() = default;
    
    reader_backend &operator=(const reader_backend &) = default;
    reader_backend &operator=(reader_backend &&) = default;

    /**
     * @brief Check wether the provided file is supported by this reader.
     * 
     * @param path Path to the file.
     * @return true File can be read by this backend.
     * @return false File cannot be read by this backend.
     */
    virtual bool supports_file(std::string_view path) const = 0;

    /**
     * @brief Create an image reader for the provided file.
     * 
     * @param path Path to the file to be read.
     * @return std::shared_ptr<reader> Newly created reader.
     */
    virtual std::shared_ptr<reader> 
    create_reader(std::string_view path) const = 0;

};

} // namespace image
} // namespace xmipp4
