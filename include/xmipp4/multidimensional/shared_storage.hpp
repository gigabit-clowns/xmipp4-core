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

#include <memory>
#include <vector>
#include <type_traits>

namespace xmipp4 
{
namespace multidimensional
{

template <typename T>
class shared_storage
{
public:
    using value_type = T;
    using alias_type = shared_storage<T>;
    using const_alias_type = shared_storage<const T>;

    shared_storage() = default;
    explicit shared_storage(std::size_t size);
    template <typename Q>
    shared_storage(const shared_storage<Q>& other);
    template <typename Q>
    shared_storage(shared_storage<Q>&& other);
    shared_storage(const shared_storage& other) = default;
    shared_storage(shared_storage&& other) = default;
    ~shared_storage() = default;

    shared_storage& operator=(const shared_storage& other) = default;
    shared_storage& operator=(shared_storage&& other) = default;

    alias_type alias() noexcept;
    const_alias_type alias() const noexcept;
    const_alias_type view() const noexcept;

    std::size_t size() const noexcept;
    void resize(std::size_t size);

    value_type* data() noexcept;
    const value_type* data() const noexcept;


private:
    using container_type = 
        std::vector<typename std::remove_const<value_type>::type>;
    std::shared_ptr<container_type> m_data;

};

template <typename T>
struct storage_traits;

template <typename T>
struct storage_traits<shared_storage<T>>
{
    using alias_type = typename shared_storage<T>::alias_type;
    using const_alias_type = typename shared_storage<T>::const_alias_type;
};

} // namespace multidimensional
} // namespace xmipp4

#include "shared_storage.inl"
