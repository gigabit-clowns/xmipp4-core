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
 * @file host_buffer_visitor.hpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Utilities to execute templated functions with
 * host_buffers.
 * @date 2024-10-29
 * 
 */

#include "host_buffer.hpp"
#include "numerical_type.hpp"
#include "../span.hpp"

namespace xmipp4 
{
namespace compute
{

/**
 * @brief Get a reference to the data of the buffer.
 * 
 * @tparam T Numerical type.
 * @param buffer The buffer.
 * @return span<T> span referencing the data of the provided buffer.
 * 
 * @warning This method does not check if the numerical type of the
 * buffer is compatible with T.
 * 
 */
template <typename T>
span<T> make_span(host_buffer &buffer) noexcept;

/**
 * @brief Get a reference to the data of the buffer.
 * 
 * @tparam T Numerical type.
 * @param buffer The buffer.
 * @return span<T> span referencing the data of the provided buffer.
 * 
 * @warning This method does not check if the numerical type of the
 * buffer is compatible with T.
 * 
 */
template <typename T>
span<const T> make_span(const host_buffer &buffer) noexcept;

/**
 * @brief Call a function with the typed data of the buffers.
 * 
 * Each of the provided buffers is converted into an appropiate
 * span according to their type and constant qualification. Then,
 * the provided function is invoked with these typed spans.
 * 
 * @tparam Func Function type.
 * @tparam Buffers Possibly cv-qualified host_buffer-s.
 * @param func The function to be evaluated with the typed buffers.
 * Should accept as many span<T> as buffers passed to the function.
 * @param buffers Buffers to be visited.
 * @return auto Invoke result of func(make_span(buffers)...)
 * 
 */
template <typename Func, typename... Buffers>
auto visit_buffers(Func &&func, Buffers&&... buffers);

/**
 * @brief Call a function with homogeneously typed buffers.
 * 
 * Each of the provided buffers is converted into an appropiate
 * span according to provided type and their constant qualification. 
 * Then, the provided function is invoked with these typed spans.
 * 
 * @tparam Func Function type
 * @tparam Buffers Possibly cv-qualified host_buffer-s.
 * @param func The function to be evaluated with the typed buffers.
 * @param type numerical_type that is assumed to be consistent across all
 * buffers
 * @param buffers Buffers to be visited.
 * @return auto Invoke result of func(make_span(buffers)...)
 * @warning This method does not check if the numerical type of each of the
 * buffers is the same as the provided type. The caller should enforce this
 * consistency before calling this function.
 * 
 */
template <typename Func, typename... Buffers>
auto visit_homogeneous_buffers(Func &&func, 
                               numerical_type type, 
                               Buffers&&... buffers );

} // namespace compute
} // namespace xmipp4

#include "host_buffer_visitor.inl"
