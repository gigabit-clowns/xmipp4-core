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

namespace xmipp4
{
namespace utils
{

template<typename RandomIt1, typename RandomIt2>
void index_sort(RandomIt1 first_index, RandomIt1 last_index, RandomIt2 first_value)
{
    // Generate indices
    using index_type = typename std::iterator_traits<RandomIt2>::value_type;
    std::iota(first_index, last_index, index_type(0));

    std::sort(
        first_index, last_index,
        [&first_value](index_type left, index_type right)
        {
            return first_value[left] < first_value[right];
        }
    );
}

template<typename RandomIt, typename IndexIt>
void permute_from(RandomIt1 first_value, RandomIt1 last_value, RandomIt2 first_index)
{
    // Based on:
    // https://stackoverflow.com/a/15625572
    using std::swap;
    using std::iter_swap;

    for(auto ite = first_value; ite != last_value; ++ite)
    {
        const auto i = std::distance(first_value, ite);
        auto j = first_index[i];
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
