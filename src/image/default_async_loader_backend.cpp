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
 * @file default_async_loader_backend.cpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Implementation of default_async_loader_backend.hpp
 * @date 2024-10-25
 * 
 */

#include "default_async_loader_backend.hpp"

#include "default_async_loader.hpp"

#include <xmipp4/core/core_version.hpp>
#include <xmipp4/core/image/async_loader_manager.hpp>
#include <xmipp4/core/logger.hpp>



namespace xmipp4
{
namespace image
{

std::string default_async_loader_backend::get_name() const noexcept
{
    return "default";
}

version default_async_loader_backend::get_version() const noexcept
{
    return get_core_version();
}

bool default_async_loader_backend::is_available() const noexcept
{
    return true;
}

backend_priority default_async_loader_backend::get_priority() const noexcept
{
    return backend_priority::preferred;
}

std::shared_ptr<async_loader> 
default_async_loader_backend::create_async_loader(const reader_manager &reader_manager) const
{
    std::size_t max_open = 64; // TODO deduce
    return std::make_shared<default_async_loader>(reader_manager, max_open);
}

bool default_async_loader_backend::register_at(async_loader_manager &manager)
{
    return manager.register_backend(
        std::make_unique<default_async_loader_backend>()
    );
}

} // namespace image
} // namespace xmipp4
