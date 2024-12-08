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
 * @file host_storage.hpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Defines host_storage class
 * @date 2024-12-08
 * 
 */

#include <xmipp4/core/compute/host_memory_allocator.hpp>
#include <xmipp4/core/compute/host_buffer.hpp>

#include <memory>
#include <typeindex>

namespace xmipp4 
{
namespace multidimensional
{

template <typename T>
class host_storage
{
public:
    using value_type = T;
    using pointer_type = value_type*;
    using const_pointer_type = const value_type*;

    explicit host_storage(compute::host_memory_allocator &allocator);
    host_storage(std::size_t size, compute::host_memory_allocator &allocator);

    compute::host_memory_allocator& get_allocator() const noexcept;

    void resize(std::size_t size);
    std::size_t get_size() const noexcept;

    pointer_type get_data() noexcept;
    const_pointer_type get_data() const noexcept;

    static std::type_index get_data_type() noexcept;

private:
    compute::host_memory_allocator* m_allocator;
    std::size_t m_size;
    std::size_t m_offset_bytes;
    std::shared_ptr<compute::host_buffer> m_buffer;

};

} // namespace multidimensional
} // namespace xmipp4

#include "host_storage.inl"
