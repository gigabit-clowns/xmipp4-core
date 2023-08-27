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

#include "label_mapping.hpp"

#include <tuple>
#include <algorithm>

namespace xmipp4
{
namespace metadata
{

inline
label_mapping::label_mapping(const std::vector<std::string>& labels)
    : m_labels(labels)
{
    compute_label_to_index_map();
}

inline
label_mapping::label_mapping(std::vector<std::string>&& labels)
    : m_labels(std::move(labels))
{
    compute_label_to_index_map();
}

inline
label_mapping::label_mapping(std::initializer_list<std::string> init)
    : m_labels(init)
{
    compute_label_to_index_map();
}

template<typename ForwardIt>
inline
label_mapping::label_mapping(ForwardIt first, ForwardIt last)
    : m_labels(first, last)
{
    compute_label_to_index_map();
}

inline
std::size_t label_mapping::operator()(const std::string& label) const
{
    return m_label_to_index.at(label);
}

inline
void label_mapping::set_labels(const std::vector<std::string>& labels)
{
    m_labels = labels;
    compute_label_to_index_map();
}

inline
void label_mapping::set_labels(std::vector<std::string>&& labels)
{
    m_labels = std::move(labels);
    compute_label_to_index_map();
}

inline
const std::vector<std::string>& label_mapping::get_labels() const noexcept
{
    return m_labels;
}

template<typename ForwardIt>
inline
void label_mapping::add_labels(ForwardIt first, ForwardIt last)
{
    auto inserted_ite = m_labels.insert(m_labels.cend(), first, last);
    update_label_to_index_map(inserted_ite);
}

inline
bool label_mapping::add_label(const std::string& label)
{
    auto inserted_ite = m_labels.insert(m_labels.cend(), label);
    update_label_to_index_map(inserted_ite);
}

inline
bool label_mapping::add_label(std::string&& label)
{
    auto inserted_ite = m_labels.insert(m_labels.cend(), std::move(label));
    update_label_to_index_map(inserted_ite);
}

inline
void label_mapping::compute_label_to_index_map()
{
    m_label_to_index.clear();
    update_label_to_index_map(m_labels.begin());
}

inline
void label_mapping::update_label_to_index_map(std::vector<std::string>::iterator first)
{
    auto& label_to_index = m_label_to_index; // Shorthand

    try
    {
        std::size_t index = std::distance(m_labels.begin(), first);

        // Remove duplicates while inserting items
        // into the map
        auto last = std::remove_if(
            first, m_labels.end(),
            [&label_to_index, index] (const auto& label) mutable -> bool
            {
                // Try to insert the string and index pair on the result map
                bool inserted;
                std::tie(std::ignore, inserted) = label_to_index.emplace(
                    label, index
                );

                if(inserted)
                    ++index;

                // Remove if duplicate (not inserted)
                return !inserted;
            }
        );

        // Erase duplicated elements
        m_labels.erase(last, m_labels.end());
    }
    catch(...)
    {
        // An exception occurred while computing the map
        // Ensure that the map and vector remain coherent
        m_labels.erase(
            std::next(m_labels.begin(), label_to_index.size()),
            m_labels.end()
        );
        throw; // Rethrow
    }
}

bool operator==(const label_mapping& x, const label_mapping& y) noexcept
{
    return x.get_labels() == y.get_labels();
}

bool operator!=(const label_mapping& x, const label_mapping& y) noexcept
{
    return x.get_labels() != y.get_labels();
}

} // namespace metadata
} // namespace xmipp4

