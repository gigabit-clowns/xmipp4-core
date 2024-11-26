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
 * @file backend.cpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Implementation of backend.hpp
 * @date 2024-11-26
 * 
 */

#include <xmipp4/core/backend.hpp>

#include <xmipp4/core/exceptions/ambiguous_backend_error.hpp>

#include <algorithm>

namespace xmipp4
{

backend* get_highest_priority_backend(span<backend*> backends)
{
    backend* result;

    if( backends.empty())
    {
        result = nullptr;
    }
    else if (backends.size() == 1)
    {
        result = backends[0];
    }
    else // available_backends.size() > 1
    {
        // Get the best two
        std::array<backend*, 2> best_two;
        std::partial_sort_copy(
            backends.begin(),
            backends.end(),
            best_two.begin(),
            best_two.end(),
            [] (const backend *lhs, const backend *rhs)
            {
                return lhs->get_priority() > rhs->get_priority();
            }
        );

        // Ensure the second is worse
        if (best_two[0]->get_priority() == best_two[1]->get_priority())
        {
            throw ambiguous_backend_error(
                "Could not disambiguate among multiple backends. "
                "Ensure that only one has been installed."
            );
        }

        result = best_two[0];
    }

    return result;
}

} // namespace xmipp4
