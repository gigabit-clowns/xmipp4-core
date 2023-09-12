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

#include <string>
#include <string_view>
#include <list>
#include <unordered_map>

namespace xmipp4
{
namespace metadata
{

template <typename T>
class label_map
{
public:
    using key_type = std::string;
    using key_view_type = std::string_view;
    using mapped_type = T;
    using value_type = std::pair<const key_type, mapped_type>;
    using container_type = std::list<value_type>;
    using size_type = typename container_type::size_type;
    using iterator =  typename container_type::iterator;
    using const_iterator =  typename container_type::const_iterator;
    using reverse_iterator =  typename container_type::reverse_iterator;
    using const_reverse_iterator =  typename container_type::const_reverse_iterator;
    using insertion_result = std::pair<iterator, bool>;
    using multiple_insertion_result = std::pair<iterator, size_type>;

    label_map() = default;
    template <typename InputIt>
    label_map(InputIt first, InputIt last);
    label_map(std::initializer_list<value_type> init);
    label_map(const label_map& other);
    label_map(label_map&& other) = default;
    ~label_map() = default;

    label_map& operator=(const label_map& other);
    label_map& operator=(label_map&& other) = default;

    iterator begin() noexcept;
    const_iterator begin() const noexcept;
    const_iterator cbegin() const noexcept;
    iterator end() noexcept;
    const_iterator end() const noexcept;
    const_iterator cend() const noexcept;
    
    reverse_iterator rbegin() noexcept;
    const_reverse_iterator rbegin() const noexcept;
    const_reverse_iterator crbegin() const noexcept;
    reverse_iterator rend() noexcept;
    const_reverse_iterator rend() const noexcept;
    const_reverse_iterator crend() const noexcept;

    size_type size() const noexcept;
    bool empty() const noexcept;
    void clear() noexcept;

    insertion_result insert(const_iterator position, const value_type& value);
    insertion_result insert(const_iterator position, value_type&& value);
    template <typename InputIt>
    multiple_insertion_result insert(const_iterator position, InputIt first, InputIt last);
    multiple_insertion_result insert(const_iterator position, std::initializer_list<value_type> init);

    multiple_insertion_result splice(const_iterator position, label_map& other);
    multiple_insertion_result splice(const_iterator position, label_map&& other);
    insertion_result splice(const_iterator position, label_map& other, const_iterator item);
    insertion_result splice(const_iterator position, label_map&& other, const_iterator item);
    multiple_insertion_result splice(const_iterator position, label_map& other, const_iterator first, const_iterator last);
    multiple_insertion_result splice(const_iterator position, label_map&& other, const_iterator first, const_iterator last);

    template<typename... Args>
    insertion_result emplace(const_iterator position, Args&&... args);

    iterator erase(const_iterator position) noexcept;
    iterator erase(const_iterator first, const_iterator last) noexcept;

    insertion_result push_back(const value_type& value);
    insertion_result push_back(value_type&& value);
    template<typename... Args>
    insertion_result emplace_back(Args&&... args);

    insertion_result push_front(const value_type& value);
    insertion_result push_front(value_type&& value);
    template<typename... Args>
    insertion_result emplace_front(Args&&... args);

    void pop_back() noexcept;
    void pop_front() noexcept;

    void swap_ordering(const_iterator x, const_iterator y) noexcept;

private:
    using key_to_position_map_type = std::unordered_map<key_view_type, typename container_type::iterator>;

    container_type m_items;
    key_to_position_map_type m_key_to_position;

    insertion_result insert_mapping(iterator position);
    multiple_insertion_result insert_mapping(iterator first, const_iterator last);

};

template<typename T>
bool operator==(const label_map<T>& left, const label_map<T>& right) noexcept;
template<typename T>
bool operator!=(const label_map<T>& left, const label_map<T>& right) noexcept;

} // namespace metadata
} // namespace xmipp4

#include "label_map.inl"

