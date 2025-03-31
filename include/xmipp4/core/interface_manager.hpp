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
 * @file interface_manager.hpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Defines interface_manager class
 * @date 2024-10-23
 * 
 */

#include "platform/dynamic_shared_object.h"

namespace xmipp4
{

/**
 * @brief Abstract class defining the interface of an interface manager.
 * 
 * An interface manager keeps track of all known implementations of a given
 * interface and provides utilities to access the most relevant implementation
 * for a given context. This is a base class for collecting them on an
 * interface_registry.
 * 
 * @see interface_registry
 * 
 */
class XMIPP4_CORE_API interface_manager
{
public:
    interface_manager() = default;
    interface_manager(const interface_manager& other) = default;
    interface_manager(interface_manager&& other) = default;
    virtual ~interface_manager() = default;

    interface_manager& operator=(const interface_manager& other) = default;
    interface_manager& operator=(interface_manager&& other) = default;

    /**
     * @brief Register backends bundled with the core.access_flag_bits
     * 
     * This usually loads fallaback implementations that are always 
     * available although they may not be the most efficient or performant.
     * The loaded backend(s) depend on the implementation of the specific
     * interface.
     * 
     */
    virtual void register_builtin_backends() = 0;

};

} // namespace xmipp4
