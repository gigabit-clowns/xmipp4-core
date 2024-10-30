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
 * @file reduction_operation.hpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Defines the reduction_operation enum.
 * @date 2024-10-24
 * 
 */

#include "platform/constexpr.hpp"

#include <cstddef>
#include <string_view>

namespace xmipp4
{
namespace communication
{

/**
 * @brief Operation to be performed when reducing a vector into a scalar
 * value.
 * 
 */
enum class reduction_operation
{
    sum, ///< Add all the elements of the vector.
    product, ///< Multiply all the elements of the vector.
    min, ///< Keep the smallest element of the vector.
    max ///< Keep the largest element of the vector.
};


XMIPP4_CONSTEXPR const char* to_string(reduction_operation op) noexcept;
bool from_string(std::string_view str, reduction_operation& op) noexcept;

} // namespace communication
} // namespace xmipp4

#include "reduction_operation.inl"
