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
    using label_to_position_map = std::unordered_map<std::string, std::size_t>;
    using label_type = label_container::value_type;
    using position_type = label_to_position_map::mapped_type;

    label_mapping() = default;
    label_mapping(const label_mapping& other) = default;
    label_mapping(label_mapping&& other) = default;
    ~label_mapping() = default;

    label_mapping& operator=(const label_mapping& other) = default;
    label_mapping& operator=(label_mapping&& other) = default;

    position_type operator()(const label_type& label) const;

    const label_container& get_labels() const noexcept;
    
    template<typename ForwardIt>
    ForwardIt add_labels(ForwardIt first, ForwardIt last);
    template<typename Label>
    bool add_label(Label&& label);

    template<typename Label>
    bool rename(position_type position, Label&& label);
    template<typename Label>
    bool rename(const label_container& old_label, Label&& new_label);

private:
    label_container m_labels;
    label_to_position_map m_label_to_position;

};

bool operator==(const label_mapping& x, const label_mapping& y) noexcept;
bool operator!=(const label_mapping& x, const label_mapping& y) noexcept;

} // namespace metadata
} // namespace xmipp4

#include "label_mapping.inl"
