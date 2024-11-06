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
 * @file host_buffer.hpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Defines the compute::host_buffer interface
 * @date 2024-11-06
 * 
 */

#include <memory>

#include "numerical_type.hpp"
#include "../platform/dynamic_shared_object.h"

namespace xmipp4 
{
namespace compute
{

/**
 * @brief Abstract class defining an in-host memory
 * allocation.
 * 
 */
class XMIPP4_CORE_API host_buffer
{
public:
    host_buffer() = default;
    host_buffer(const host_buffer &other) = default;
    host_buffer(host_buffer &&other) = default;
    virtual ~host_buffer() = default;

    host_buffer& operator=(const host_buffer &other) = default;
    host_buffer& operator=(host_buffer &&other) = default;

    /**
     * @brief Get the numeric type of the data in the buffer.
     * 
     * @return numerical_type The numeric type.
     * 
     */
    virtual numerical_type get_type() const noexcept = 0;

    /**
     * @brief Get the number of elements in the buffer.
     * 
     * @return std::size_t The number of elements.
     * @note This does not need to be confused with the number of
     * bytes that occupies this buffer.
     * 
     */
    virtual std::size_t get_count() const noexcept = 0;

    /**
     * @brief Get the data.
     * 
     * @return void* The data.
     */
    virtual void* get_data() noexcept = 0;

    /**
     * @brief Get the data.
     * 
     * @return const void* The data.
     */
    virtual const void* get_data() const noexcept = 0;

}; 

} // namespace compute
} // namespace xmipp4
