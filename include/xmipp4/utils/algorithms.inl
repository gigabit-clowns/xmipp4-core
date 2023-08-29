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

#include "algorithms.hpp"

#include <algorithm>
#include <iterator>

namespace xmipp4
{
namespace utils
{

template<typename RandomIt, typename IndexIt>
void permute_from(RandomIt first_value, IndexIt first_index, std::size_t n)
{
    // Based on:
    // https://stackoverflow.com/a/15625572
    using std::swap;
    using std::iter_swap;
    using index_type = typename std::iterator_traits<IndexIt>::value_type;

    for(std::size_t i = 0; i < n; ++i)
    {
        index_type j = first_index[i];
        first_index[i] = i;
        while(j != i)
        {
            iter_swap(first_value + j, first_value + first_index[j]);
            swap(j, first_index[j]);
        }
    }
}

} // namespace utils
} // namespace xmipp4
