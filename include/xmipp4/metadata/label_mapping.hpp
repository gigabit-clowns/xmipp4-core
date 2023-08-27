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
    label_mapping() = default;
    explicit label_mapping(const std::vector<std::string>& labels);
    explicit label_mapping(std::vector<std::string>&& labels);
    label_mapping(std::initializer_list<std::string> init);
    template<typename ForwardIt>
    label_mapping(ForwardIt first, ForwardIt last);
    label_mapping(const label_mapping& other) = default;
    label_mapping(label_mapping&& other) = default;
    ~label_mapping() = default;

    label_mapping& operator=(const label_mapping& other) = default;
    label_mapping& operator=(label_mapping&& other) = default;

    std::size_t operator()(const std::string& label) const;

    void set_labels(const std::vector<std::string>& labels);
    void set_labels(std::vector<std::string>&& labels);
    const std::vector<std::string>& get_labels() const noexcept;
    
    template<typename ForwardIt>
    void add_labels(ForwardIt first, ForwardIt last);
    bool add_label(const std::string& label);
    bool add_label(std::string&& label);

    std::size_t get_index(const std::string& label) const;

private:
    std::vector<std::string> m_labels;
    std::unordered_map<std::string, std::size_t> m_label_to_index;

    void compute_label_to_index_map();
    void update_label_to_index_map(std::vector<std::string>::iterator first);

};

bool operator==(const label_mapping& x, const label_mapping& y) noexcept;
bool operator!=(const label_mapping& x, const label_mapping& y) noexcept;

} // namespace metadata
} // namespace xmipp4

#include "label_mapping.inl"
