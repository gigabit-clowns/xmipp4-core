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
#include <vector>
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
    using mapped_type = T;
    using value_type = std::pair<const key_type, mapped_type>;
    using container_type = std::vector<value_type>;
    using key_to_index_map_type = std::unordered_map<key_type, std::size_t>;
    using size_type = typename container_type::size_type;
    using iterator =  typename container_type::iterator;
    using const_iterator =  typename container_type::const_iterator;
    using reverse_iterator =  typename container_type::reverse_iterator;
    using const_reverse_iterator =  typename container_type::const_reverse_iterator;

    label_map() = default;
    template <typename InputIt>
    label_map(InputIt first, InputIt last);
    label_map(std::initializer_list<value_type> init);
    label_map(const label_map& other) = default;
    label_map(label_map&& other) = default;
    ~label_map() = default;

    label_map& operator=(const label_map& other) = default;
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

    void reserve(size_type count);

    std::pair<iterator, bool> insert(const_iterator position, const value_type& value);
    std::pair<iterator, bool> insert(const_iterator position, value_type&& value);
    template <typename InputIt>
    std::pair<iterator, size_type> insert(const_iterator position, InputIt first, InputIt last);
    std::pair<iterator, size_type> insert(const_iterator position, std::initializer_list<value_type> init);
    template<typename... Args>
    std::pair<iterator, bool> emplace(const_iterator position, Args&&... args);
    iterator erase(const_iterator position) noexcept;
    iterator erase(const_iterator first, const_iterator last) noexcept;

    bool push_back(const value_type& value);
    bool push_back(value_type&& value);
    template<typename... Args>
    bool emplace_back(Args&&... args);

    void pop_back() noexcept;

private:
    container_type m_items;
    key_to_index_map_type m_key_to_index;

};

template<typename T>
bool operator==(const label_map<T>& left, const label_map<T>& right) noexcept;
template<typename T>
bool operator!=(const label_map<T>& left, const label_map<T>& right) noexcept;

} // namespace metadata
} // namespace xmipp4

#include "label_map.inl"

