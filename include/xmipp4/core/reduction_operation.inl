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
 * @file reduction_operation.inl
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Implementation of reduction_operation.inl
 * @date 2024-10-24
 * 
 */

#include "reduction_operation.hpp"

#include <unordered_map>

namespace xmipp4
{

XMIPP4_INLINE_CONSTEXPR 
const char* to_string(reduction_operation op) noexcept
{
    switch (op)
    {
    case reduction_operation::sum: return "sum";
    case reduction_operation::product: return "product";
    case reduction_operation::min: return "min";
    case reduction_operation::max: return "max";

    }
}

inline
bool from_string(std::string_view str, reduction_operation& op) noexcept
{
    static const
    std::unordered_map<std::string_view, reduction_operation> str_2_reduction_operation = 
    {
        { to_string(reduction_operation::sum), reduction_operation::sum },
        { to_string(reduction_operation::product), reduction_operation::product },
        { to_string(reduction_operation::min), reduction_operation::min },
        { to_string(reduction_operation::max), reduction_operation::max },

    };

    const auto ite = str_2_reduction_operation.find(str);
    const bool result = ite != str_2_reduction_operation.end();
    if(result)
        op = ite->second;
    return result;
}

} // namespace xmipp4
