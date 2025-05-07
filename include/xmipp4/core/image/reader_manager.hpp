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
 * @file reader_manager.hpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Definition of the image::reader_manager class
 * @date 2025-05-07
 * 
 */

#include "../interface_manager.hpp"
#include "../memory/pimpl.hpp"

#include <string_view>
#include <memory>
#include <cstddef>

namespace xmipp4 
{
namespace image
{

class reader;

/**
 * @brief Centralizes all known reader_backend-s.
 * 
 */
class reader_manager final
    : public interface_manager
{
public:
    reader_manager() = default;
    reader_manager(const reader_manager &) = default;
    reader_manager(reader_manager &&) = default;
    virtual ~reader_manager() = default;
    
    reader_manager &operator=(const reader_manager &) = default;
    reader_manager &operator=(reader_manager &&) = default;

    XMIPP4_CORE_API 
    void register_builtin_backends() override;

    XMIPP4_CORE_API
    bool register_backend(std::unique_ptr<reader_backend> backend);
 
    XMIPP4_CORE_API
    void enumerate_backends(std::vector<std::string> &backends) const;

    XMIPP4_CORE_API
    reader_backend* get_backend(const std::string &name) const;

    /**
     * @brief Create an image reader for the given file.
     * 
     * @param path Path to the file to be read.
     * @return std::shared_ptr<reader> Newly created reader.
     */
    XMIPP4_CORE_API 
    virtual std::shared_ptr<reader> 
    create_reader(std::string_view path) const;

private:
    class implementation;
    memory::pimpl<implementation> m_implementation;

};

} // namespace image
} // namespace xmipp4
