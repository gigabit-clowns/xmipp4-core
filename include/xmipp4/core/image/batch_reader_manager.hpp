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
 * @file batch_reader_manager.hpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Definition of the image::batch_reader_manager class
 * @date 2025-05-07
 * 
 */

#include "../interface_manager.hpp"

#include <string_view>
#include <memory>
#include <vector>
#include <cstddef>

namespace xmipp4 
{
namespace image
{

class batch_reader;
class batch_reader_backend;
class reader_manager;

/**
 * @brief Centralizes all known batch_reader_backend-s.
 * 
 */
class batch_reader_manager final
    : public interface_manager
{
public:
    batch_reader_manager() = default;
    batch_reader_manager(const batch_reader_manager &) = default;
    batch_reader_manager(batch_reader_manager &&) = default;
    virtual ~batch_reader_manager() = default;
    
    batch_reader_manager &operator=(const batch_reader_manager &) = default;
    batch_reader_manager &operator=(batch_reader_manager &&) = default;

    XMIPP4_CORE_API 
    void register_builtin_backends() override;

    /**
     * @brief Register a new batch_reader_backend.
     * 
     * @param backend The backend to be registered.
     * 
     */
    XMIPP4_CORE_API 
    void register_backend(std::unique_ptr<batch_reader_backend> backend);

    /**
     * @brief Enumerate all registered backends.
     * 
     * @param backends Output parameter with the names of the backends.
     * 
     */
    XMIPP4_CORE_API 
    void enumerate_backends(std::vector<std::string> &backends) const;

    /**
     * @brief Create a new batch reader object.
     * 
     * @param backend Name of the backend to be used.
     * @param readers Reader manager to be used by the batch reader.
     * @return std::shared_ptr<batch_reader> Newly created batch reader.
     * 
     */
    XMIPP4_CORE_API 
    std::shared_ptr<batch_reader> 
    create_batch_reader(const std::string &backend, 
                        const reader_manager &readers ) const;

private:

};

} // namespace image
} // namespace xmipp4
