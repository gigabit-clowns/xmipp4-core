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
 * @file lru_batch_reader_backend.cpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Implementation of lru_batch_reader_backend.hpp
 * @date 2024-10-25
 * 
 */

#include "lru_batch_reader_backend.hpp"

#include "lru_batch_reader.hpp"

#include <xmipp4/core/core_version.hpp>
#include <xmipp4/core/image/batch_reader_manager.hpp>

static const char XMIPP4_LRU_READER_MAX_OPEN[] = 
    "XMIPP4_LRU_READER_MAX_OPEN";

namespace xmipp4
{
namespace image
{

std::string lru_batch_reader_backend::get_name() const noexcept
{
    return "lru";
}

version lru_batch_reader_backend::get_version() const noexcept
{
    return get_core_version();
}

bool lru_batch_reader_backend::is_available() const noexcept
{
    return true;
}

backend_priority lru_batch_reader_backend::get_priority() const noexcept
{
    return backend_priority::preferred;
}

std::shared_ptr<batch_reader> 
lru_batch_reader_backend::create_batch_reader(const reader_manager &reader_manager) const
{
    const char* max_open_str = std::getenv(XMIPP4_LRU_READER_MAX_OPEN);
    std::size_t max_open = 64;
    if (max_open_str)
    {
        // TODO parse
    }

    return std::make_shared<lru_batch_reader>(reader_manager, max_open);
}

bool lru_batch_reader_backend::register_at(batch_reader_manager &manager)
{
    return manager.register_backend(
        std::make_unique<lru_batch_reader_backend>()
    );
}

} // namespace image
} // namespace xmipp4
