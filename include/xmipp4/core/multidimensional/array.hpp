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
 * @file dynamic_layout.hpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Defines dynamic_layout class
 * @date 2025-02-03
 * 
 */

#include <vector>
#include <memory>
#include <cstddef>

#include "axis_descriptor.hpp"

namespace xmipp4 
{
namespace multidimensional
{

template <typename T>
struct storage_traits;

template <typename T>
struct layout_traits;



template <typename Storage, typename Layout>
class array
{
public:
    using storage_type = Storage;
    using layout_type = Layout;

private:
    storage_type m_storage;
    layout_type m_layout;

};

} // namespace multidimensional
} // namespace xmipp4
