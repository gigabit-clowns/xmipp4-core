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
 * @file batch_loader_manager.cpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Implementation of batch_loader_manager.hpp
 * @date 2024-10-25
 * 
 */

#include <xmipp4/core/image/batch_loader_manager.hpp>

#include <xmipp4/core/image/batch_loader_backend.hpp>

#include "lru_batch_loader_backend.hpp"

namespace xmipp4
{
namespace image
{

static 
std::shared_ptr<batch_loader> 
create_batch_loader(const batch_loader_backend* backend, 
                    const reader_manager &reader_manager )
{
    std::shared_ptr<batch_loader> result;

    if (backend)
    {
        result = backend->create_batch_loader(reader_manager);
    }

    return result;
}

void batch_loader_manager::register_builtin_backends() 
{
    lru_batch_loader_backend::register_at(*this);
}

std::shared_ptr<batch_loader> 
batch_loader_manager::create_batch_loader(const std::string &backend_name,
                                          const reader_manager &reader_manager ) const
{
    const auto *backend = get_backend(backend_name);
    return image::create_batch_loader(backend, reader_manager);
}

} // namespace image
} // namespace xmipp4
