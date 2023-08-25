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

#include <xmipp4/metadata/label_mapping.hpp>

#include <xmipp4/platform/assert.hpp>

namespace xmipp4
{
namespace metadata
{

label_mapping::label_mapping(std::vector<std::string> labels)
    : m_labels(std::move(labels))
    , m_label_to_index(compute_index_mapping(m_labels))
{
}

void label_mapping::set_labels(std::vector<std::string> labels)
{
    auto label_to_index = compute_index_mapping(labels);
    m_labels = std::move(labels);
    m_label_to_index = std::move(label_to_index);
}

const std::vector<std::string>& label_mapping::get_labels() const noexcept
{
    return m_labels;
}   

void label_mapping::reserve(std::size_t capacity)
{
    m_labels.reserve(capacity);
    m_label_to_index.reserve(capacity);
}

bool label_mapping::add_label(std::string label)
{
    // Add the label
    const auto prev_capacity = m_labels.capacity();
    m_labels.push_back(std::move(label));

    bool inserted;
    if(m_labels.capacity() != prev_capacity)
    {
        // Reallocation ocurred. String views may be invalidated.
        // Thus, re-compute index mapping
        try
        {
            const auto prev_size = m_labels.size();
            m_label_to_index = compute_index_mapping(m_labels);
            inserted = m_labels.size() == prev_size;
        }
        catch(...)
        {
            // An exception occurred. Undo changes
            m_labels.pop_back();
            throw; // Propagate
        }
    }
    else
    {
        // Allocation persists. Current index mapping remains
        // valid. Add the new element
        try
        {
            const auto index = m_labels.size() - 1;
            std::tie(std::ignore, inserted) = m_label_to_index.emplace(
                m_labels[index], index
            );
        }
        catch(...)
        {
            // An exception occurred. Undo changes
            m_labels.pop_back();
            throw; // Propagate
        }

        if(!inserted)
        {
            // Element already exists. Remove from label list
            m_labels.pop_back();
        }
    }

    XMIPP4_ASSERT(m_labels.size() == m_label_to_index.size());
    return inserted;
}

std::size_t label_mapping::get_index(std::string_view label) const
{
    return m_label_to_index.at(label);
}

label_mapping::index_map label_mapping::compute_index_mapping(std::vector<std::string>& labels)
{
    index_map result;
    result.reserve(labels.size());

    auto ite = labels.cbegin();
    while(ite != labels.cend())
    {   
        // Try to insert the string and index pair on the result
        bool inserted;
        std::tie(std::ignore, inserted) = result.emplace(
            *ite, std::distance(labels.cbegin(), ite)
        );

        if(inserted)
            ++ite;
        else
            ite = labels.erase(ite); // Label already exits. Remove
    }

    XMIPP4_ASSERT(result.size() == labels.size());
    return result;
}

} // namespace metadata
} // namespace xmipp4
