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

#include "label_mapping.hpp"

#include <tuple>
#include <algorithm>

namespace xmipp4
{
namespace metadata
{

template<typename ForwardIt, typename Map>
inline
ForwardIt update_label_to_position_map(ForwardIt first, 
                                       ForwardIt last, 
                                       Map& map, 
                                       typename Map::mapped_type start )
{
    return std::remove_if(
        first, last,
        [&map, &start] (const auto& labeled) -> bool
        {
            // Try to insert the string and index pair on the result map
            bool inserted;
            std::tie(std::ignore, inserted) = map.emplace(
                labeled->get_label(), start
            );

            // Increment the current index
            if(inserted)
                ++start;

            // Remove if duplicate (not inserted)
            return !inserted;
        }
    );
}

} // namespace metadata
} // namespace xmipp4

