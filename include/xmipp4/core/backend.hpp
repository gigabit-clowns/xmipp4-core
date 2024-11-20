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
 * @file backend.hpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Defines basic backend interface
 * @date 2024-10-29
 * 
 */

#include "version.hpp"
#include "backend_priority.hpp"
#include "platform/dynamic_shared_object.h"

#include <string>

namespace xmipp4
{

/**
 * @brief Abstract class representing a backend.
 * 
 * A concrete instance of this class may be used to create objects and query
 * an specific backend.
 * 
 */
class XMIPP4_CORE_API backend
{
public:
    backend() = default;
    backend(const backend& other) = default;
    backend(backend&& other) = default;
    virtual ~backend() = default;

    backend& operator=(const backend& other) = default;
    backend& operator=(backend&& other) = default;

    /**
     * @brief Get the name of the backend.
     * 
     * @return std::string The name.
     */
    virtual std::string get_name() const noexcept = 0;

    /**
     * @brief Get the version of the backend.
     * 
     * @return version The version.
     */
    virtual version get_version() const noexcept = 0;

    /**
     * @brief Check if a backend is usable.
     * 
     * @return true Backend is usable.
     * @return false Backend is not usable.
     */
    virtual bool is_available() const noexcept = 0;

    /**
     * @brief Get the priority of this backend.
     * 
     * This attribute can be used to break a tie when multiple backends
     * can be used. In general prefer using backend_priority::normal unless 
     * there is a strong reason.
     * 
     * @return backend_priority The priority of this backend.
     */
    virtual backend_priority get_priority() const noexcept = 0;

};

} // namespace xmipp4
