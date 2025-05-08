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
 * @file writer_backend.hpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Definition of the image::writer_backend class
 * @date 2025-05-07
 * 
 */

#include "../backend.hpp"

#include <string>
#include <memory>
#include <cstddef>

namespace xmipp4 
{
namespace image
{

class writer;

/**
 * @brief Abstract backend class to create image writers. 
 * 
 */
class writer_backend
    : public backend
{
public:
    writer_backend() = default;
    writer_backend(const writer_backend &) = default;
    writer_backend(writer_backend &&) = default;
    virtual ~writer_backend() = default;
    
    writer_backend &operator=(const writer_backend &) = default;
    writer_backend &operator=(writer_backend &&) = default;

    /**
     * @brief Check wether the provided path is supported by this writer 
     * backend.
     * 
     * @param path Path to the file.
     * @return true File can be write by this backend.
     * @return false File cannot be write by this backend.
     */
    virtual bool supports_file(const std::string &path) const = 0;

    /**
     * @brief Create an image writer for the provided file.
     * 
     * @param path Path to the file to be write.
     * @return std::shared_ptr<writer> Newly created writer.
     */
    virtual std::shared_ptr<writer> 
    create_writer(const std::string &path) const = 0; // TODO parameters

};

} // namespace image
} // namespace xmipp4
