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
 * @file writer_manager.hpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Definition of the image::writer_manager class
 * @date 2025-05-07
 * 
 */

#include "writer_backend.hpp"
#include "../backend_manager.hpp"

#include <string>
#include <cstddef>

namespace xmipp4 
{
namespace image
{

class writer;

/**
 * @brief Centralizes all known writer_backend-s.
 * 
 */
class writer_manager final
    : public basic_backend_manager<writer_backend>
{
public:
    writer_manager() = default;
    writer_manager(const writer_manager &) = default;
    writer_manager(writer_manager &&) = default;
    virtual ~writer_manager() = default;
    
    writer_manager &operator=(const writer_manager &) = default;
    writer_manager &operator=(writer_manager &&) = default;

    /**
     * @brief Get a suitable writer backend for for a path.
     * 
     * @param path Path to an image file.
     * @return writer_backend* A suitable backend. nullptr if not found.
     */
    XMIPP4_CORE_API
    writer_backend* get_backend_for_file(const std::string &path) const;

    /**
     * @brief Create an image writer for the given file.
     * 
     * @param path Path to the file to be read.
     * @return std::shared_ptr<writer> Newly created writer.
     */
    XMIPP4_CORE_API 
    virtual std::shared_ptr<writer> 
    create_writer(const std::string &path) const; // TODO parameters

};

} // namespace image
} // namespace xmipp4
