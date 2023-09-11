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

class label_mapping
{
public:
    using label_container = std::vector<std::string>;
    using label_to_index_map = std::unordered_map<std::string, std::size_t>;
    using label_type = label_container::value_type;
    using index_type = label_to_index_map::mapped_type;
    using const_iterator = label_container::const_iterator;

    label_mapping() = default;
    label_mapping(const label_mapping& other) = default;
    label_mapping(label_mapping&& other) = default;
    ~label_mapping() = default;

    label_mapping& operator=(const label_mapping& other) = default;
    label_mapping& operator=(label_mapping&& other) = default;

    index_type operator()(const label_type& label) const;
    index_type get_index(const label_type& label) const;

    const label_container& get_labels() const noexcept;

    void clear() noexcept;
    void reserve(std::size_t size);
    std::size_t size() const noexcept;
    bool empty() const noexcept;

    const_iterator begin() const noexcept;
    const_iterator cbegin() const noexcept;
    const_iterator end() const noexcept;
    const_iterator cend() const noexcept;

    const_iterator find(const label_type& label) const noexcept;
    bool contains(const label_type& label) const noexcept;

    template<typename Label>
    bool insert(const_iterator position, Label&& label);
    template<typename Label>
    bool push_back(Label&& label);
    const_iterator erase(const_iterator position) noexcept;
    std::size_t erase(const label_type& label) noexcept;

    template<typename Label>
    bool rename(const_iterator position, Label&& label);
    template<typename Label>
    bool rename(const label_type& prev_label, Label&& label);

private:
    label_container m_labels;
    label_to_index_map m_label_to_index;

};

bool operator==(const label_mapping& x, const label_mapping& y) noexcept;
bool operator!=(const label_mapping& x, const label_mapping& y) noexcept;

} // namespace metadata
} // namespace xmipp4

#include "label_mapping.inl"
