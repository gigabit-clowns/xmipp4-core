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

namespace xmipp4
{
namespace utils
{

template<typename RandomIt1, typename RandomIt2>
void index_sort(RandomIt1 first_index, RandomIt1 last_index, RandomIt2 first_value);

template<typename RandomIt1, typename RandomIt2>
void permute_from(RandomIt1 first_value, RandomIt1 last_value, RandomIt2 first_index);


} // namespace utils
} // namespace xmipp4

#include "algorithms.inl"
