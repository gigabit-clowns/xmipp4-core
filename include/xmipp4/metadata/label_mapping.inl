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

namespace xmipp4
{
namespace metadata
{

inline
label_mapping::position_type label_mapping::operator()(const label_type& label) const
{
    return m_label_to_position.at(label);
}

inline
const label_mapping::label_container& label_mapping::get_labels() const noexcept
{
    return m_labels;
}

template<typename ForwardIt>
inline
ForwardIt label_mapping::add_labels(ForwardIt first, ForwardIt last)
{
    for(; first != last; ++first)
    {
        const auto inserted = add_label(*first);
        if(!inserted) break;
    }
    return first;
}

template<typename Label>
inline 
bool label_mapping::add_label(Label&& label)
{
    // Try to insert it on the label map
    bool inserted;
    label_to_position_map::iterator it;
    std::tie(it, inserted) = m_label_to_position.emplace(
        std::forward<Label>(label), m_labels.size()
    );

    // If inserted, add it to the label list
    if(inserted)
    {
        try
        {
            m_labels.push_back(it->first);
        }
        catch(...)
        {
            // Could not insert into the label list
            // Remove from the map entry to keep consistency
            m_label_to_position.erase(it);
            throw; //Rethrow
        }
    }

    return inserted;
}

template<typename Label>
inline
bool label_mapping::rename(position_type position, Label&& label)
{
    // Try to insert it on the label map
    bool inserted;
    label_to_position_map::iterator it;
    std::tie(it, inserted) = m_label_to_position.emplace(
        std::forward<Label>(label), position
    );

    // If inserted, update the label list and erase the
    // old mapping
    if(inserted)
    {
        try
        {
            auto& old_label = m_labels.at(position);
            auto old_it = m_label_to_position.find(old_label);

            old_label = it->first;
            m_label_to_position.erase(old_it);
        }
        catch(...)
        {
            // Could not insert into the label list
            // Remove from the map entry to keep consistency
            m_label_to_position.erase(it);
            throw; //Rethrow
        }
    }

    return inserted;
}



inline
bool operator==(const label_mapping& x, const label_mapping& y) noexcept
{
    return x.get_labels() == y.get_labels();
}

inline
bool operator!=(const label_mapping& x, const label_mapping& y) noexcept
{
    return x.get_labels() != y.get_labels();
}

} // namespace metadata
} // namespace xmipp4

