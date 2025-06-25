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
 * @file storage_reference.hpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Defines storage_reference class
 * @date 2025-03-20
 * 
 */

#include "storage_traits.hpp"

#include <memory>

namespace xmipp4 
{
namespace multidimensional
{

template <typename T>
class storage_reference
{
public:
    using storage_type = T;
    using value_type = 
        typename storage_traits<storage_type>::value_type;
    using is_host_accessible = 
        typename storage_traits<storage_type>::is_host_accessible;

    storage_reference() = default;
    template <typename... Args>
    explicit storage_reference(Args&&... args);
    storage_reference(const storage_reference &other) = default;
    storage_reference(storage_reference &&other) = default;
    storage_reference(const storage_reference<const storage_type> &other);
    storage_reference(storage_reference<const storage_type> &&other);
    ~storage_reference() = default;

    storage_reference& operator=(const storage_reference &other) = default;
    storage_reference& operator=(storage_reference &&other) = default;

    void reset() noexcept;
    void swap(storage_reference &other) noexcept;

    bool aliases(const storage_reference &other) const noexcept;

    storage_type* get() noexcept;
    const storage_type* get() const noexcept;

    std::size_t size() const noexcept;

    typename std::enable_if<is_host_accessible::value, value_type*>::type
    data() noexcept;
    typename std::enable_if<is_host_accessible::value, const value_type*>::type
    data() const noexcept;

private:
    std::shared_ptr<value_type> m_storage;

};

template <typename T>
void swap(storage_reference<T> &lhs, storage_reference<T> &rhs) noexcept;

} // namespace multidimensional
} // namespace xmipp4

#include "storage_reference.inl"
