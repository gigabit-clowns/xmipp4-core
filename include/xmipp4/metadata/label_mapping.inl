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
label_mapping::index_type label_mapping::operator()(const label_type& label) const
{
    return get_index(label);
}

inline
label_mapping::index_type label_mapping::get_index(const label_type& label) const
{
    return m_label_to_index.at(label);
}

inline
const label_mapping::label_container& label_mapping::get_labels() const noexcept
{
    return m_labels;
}

inline
void label_mapping::clear() noexcept
{
    m_labels.clear();
    m_label_to_index.clear();
}

inline
void label_mapping::reserve(std::size_t size)
{
    m_labels.reserve(size);
    m_label_to_index.reserve(size);
}

inline
std::size_t label_mapping::size() const noexcept
{
    return m_labels.size();
}

inline
bool label_mapping::empty() const noexcept
{
    return m_labels.empty();
}

inline
label_mapping::const_iterator label_mapping::begin() const noexcept
{
    return m_labels.begin();
}

inline
label_mapping::const_iterator label_mapping::cbegin() const noexcept
{
    return begin();
}

inline
label_mapping::const_iterator label_mapping::end() const noexcept
{
    return m_labels.end();
}

inline
label_mapping::const_iterator label_mapping::cend() const noexcept
{
    return end();
}

inline
label_mapping::const_iterator label_mapping::find(const label_type& label) const noexcept
{
    const auto ite = m_label_to_index.find(label);
    if (ite == m_label_to_index.end())
        return end();
    else
        return std::next(begin(), ite->second);
}

inline
bool label_mapping::contains(const label_type& label) const noexcept
{
    return m_label_to_index.find(label) != m_label_to_index.end();
}

template<typename Label>
inline
bool label_mapping::insert(const_iterator position, Label&& label)
{
    // Try to insert the new label into the mapping
    bool inserted;
    label_to_index_map::iterator mapping;
    std::tie(mapping, inserted) = m_label_to_index.emplace(
        std::forward<Label>(label), 
        std::distance(begin(), position)
    );

    if(inserted)
    {
        // Insert the label into the label list
        try
        {
            position = m_labels.emplace(position, mapping->first);
        }
        catch(...)
        {
            // Could not insert into the label list
            // Remove from the map entry to keep consistency
            m_label_to_index.erase(mapping);
            throw; //Rethrow
        }

        // Increment all the indices after it
        for(++position; position =! end(); ++position)
            ++(m_label_to_index.find(*position)->second);
    }

    return inserted;
}

template<typename Label>
inline
bool label_mapping::push_back(Label&& label)
{
    // Try to insert the new label into the mapping
    bool inserted;
    label_to_index_map::iterator mapping;
    std::tie(mapping, inserted) = m_label_to_index.emplace(
        std::forward<Label>(label), 
        m_labels.size()
    );

    if(inserted)
    {
        // Insert the label into the label list
        try
        {
            m_labels.emplace_back(mapping->first);
        }
        catch(...)
        {
            // Could not insert into the label list
            // Remove from the map entry to keep consistency
            m_label_to_index.erase(mapping);
            throw; //Rethrow
        }
    }

    return inserted;
}

inline
label_mapping::const_iterator label_mapping::erase(const_iterator position) noexcept
{
    // Erase from the mapping
    m_label_to_index.erase(*position);

    // Erase from the label list
    const auto result = m_labels.erase(position);

    // Decrement all the indices after the erased item
    for(auto ite = result; ite != end(); ++ite)
        --(m_label_to_index.find(*ite)->second);
    
    return result;
}

inline
std::size_t label_mapping::erase(const label_type& label) noexcept
{
    std::size_t result = 0;

    // Find the element to be erased
    const auto ite = m_label_to_index.find(label);
    if (ite != m_label_to_index.end())
    {
        // Delete from the label list
        m_labels.erase(std::next(m_labels.begin(), ite->second));

        // Delete from the mapping
        m_label_to_index.erase(ite);

        result = 1;
    }

    return result;
}

template<typename Label>
bool label_mapping::rename(const_iterator position, Label&& label)
{
    // Try to insert the new label into the mapping
    bool inserted;
    label_to_index_map::iterator mapping;
    std::tie(mapping, inserted) = m_label_to_index.emplace(
        std::forward<Label>(label), 
        std::distance(begin(), position)
    );

    if(inserted)
    {
        // Update the label list
        try
        {
            const auto prev_mapping = m_label_to_index.find(*position);
            m_labels[prev_mapping->second] = mapping->first;
            m_label_to_index.erase(prev_mapping);
        }
        catch(...)
        {
            // Could not insert into the label list
            // Remove from the map entry to keep consistency
            m_label_to_index.erase(mapping);
            throw; //Rethrow
        }
    }

    return inserted;
}

template<typename Label>
inline
bool label_mapping::rename(const label_type& prev_label, Label&& label)
{
    bool inserted = false;

    const auto prev_mapping = m_label_to_index.find(prev_label);
    if (prev_mapping != m_label_to_index.end())
    {   
        // Try to insert the new label into the mapping
        label_to_index_map::iterator mapping;
        std::tie(mapping, inserted) = m_label_to_index.emplace(
            std::forward<Label>(label), 
            prev_mapping->second
        );

        if(inserted)
        {
            // Update the label list
            try
            {
                m_labels[prev_mapping->second] = mapping->first;
                m_label_to_index.erase(prev_mapping);
            }
            catch(...)
            {
                // Could not insert into the label list
                // Remove from the map entry to keep consistency
                m_label_to_index.erase(mapping);
                throw; //Rethrow
            }
        }
    }

    return inserted;
}



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

