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
 * @file batch_loader_manager.hpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Definition of the image::batch_loader_manager class
 * @date 2025-05-07
 * 
 */
 
#include "../backend_manager.hpp"
#include "../platform/dynamic_shared_object.h"

#include <string>
#include <cstddef>

namespace xmipp4 
{
namespace image
{

class reader_manager;
class batch_loader;
class batch_loader_backend;

/**
 * @brief Centralizes all known batch_loader_backend-s.
 * 
 */
class batch_loader_manager final
    : public basic_backend_manager<batch_loader_backend>
{
public:
    batch_loader_manager() = default;
    batch_loader_manager(const batch_loader_manager &) = default;
    batch_loader_manager(batch_loader_manager &&) = default;
    virtual ~batch_loader_manager() = default;
    
    batch_loader_manager &operator=(const batch_loader_manager &) = default;
    batch_loader_manager &operator=(batch_loader_manager &&) = default;

    XMIPP4_CORE_API
    void register_builtin_backends() override;

    XMIPP4_CORE_API
    std::shared_ptr<batch_loader> 
    create_batch_loader(const std::string &backend_name,
                        const reader_manager &reader_manager) const;

};

} // namespace image
} // namespace xmipp4
