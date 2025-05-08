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
 * @file mock_backend.hpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Mock for backend interface.
 * @date 2025-03-27
 */

#include <xmipp4/core/backend.hpp>

#include <trompeloeil.hpp>

namespace xmipp4
{

class mock_backend final
    : public backend
{
public:
    MAKE_MOCK0(get_name, std::string(), const noexcept);
    MAKE_MOCK0(get_version, version(), const noexcept);
    MAKE_MOCK0(is_available, bool(), const noexcept);
    MAKE_MOCK0(get_priority, backend_priority(), const noexcept);

};

} // namespace xmipp4
