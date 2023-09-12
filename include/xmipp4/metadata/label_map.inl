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

#include "label_map.hpp"

namespace xmipp4
{
namespace metadata
{

template <typename T>
template <typename InputIt>
inline
label_map<T>::label_map(InputIt first, InputIt last)
{
    for(auto it = first; it != last; ++it)
        push_back(*it);
}

template <typename T>
inline
label_map<T>::label_map(std::initializer_list<value_type> init)
    : label_map(init.begin(), init.end())
{
}

template <typename T>
inline
typename label_map<T>::iterator label_map<T>::begin() noexcept
{
    return m_items.begin();
}

template <typename T>
inline
typename label_map<T>::const_iterator label_map<T>::begin() const noexcept
{
    return m_items.begin();
}

template <typename T>
inline
typename label_map<T>::const_iterator label_map<T>::cbegin() const noexcept
{
    return begin();
}

template <typename T>
inline
typename label_map<T>::iterator label_map<T>::end() noexcept
{
    return m_items.end();
}

template <typename T>
inline
typename label_map<T>::const_iterator label_map<T>::end() const noexcept
{
    return m_items.end();
}

template <typename T>
inline
typename label_map<T>::const_iterator label_map<T>::cend() const noexcept
{
    return end();
}

template <typename T>
inline
typename label_map<T>::reverse_iterator label_map<T>::rbegin() noexcept
{
    return m_items.rbegin();
}

template <typename T>
inline
typename label_map<T>::const_reverse_iterator label_map<T>::rbegin() const noexcept
{
    return m_items.rbegin();
}

template <typename T>
inline
typename label_map<T>::const_reverse_iterator label_map<T>::crbegin() const noexcept
{
    return rbegin();
}

template <typename T>
inline
typename label_map<T>::reverse_iterator label_map<T>::rend() noexcept
{
    return m_items.rend();
}

template <typename T>
inline
typename label_map<T>::const_reverse_iterator label_map<T>::rend() const noexcept
{
    return m_items.rend();
}

template <typename T>
inline
typename label_map<T>::const_reverse_iterator label_map<T>::crend() const noexcept
{
    return rend();
}

template <typename T>
inline
typename label_map<T>::size_type label_map<T>::size() const noexcept
{
    return m_items.size();
}

template <typename T>
inline
bool label_map<T>::empty() const noexcept
{
    return m_items.empty();
}

template <typename T>
inline
void label_map<T>::clear() noexcept
{
    m_items.clear();
    m_key_to_index.clear();
}

template <typename T>
inline
void label_map<T>::reserve(size_type count)
{
    m_items.reserve(count);
    m_key_to_index.reserve(count);
}

template <typename T>
inline
std::pair<typename label_map<T>::iterator, bool>
label_map<T>::insert(const_iterator position, const value_type& value)
{
    // Try to insert the new key
    iterator new_position;
    key_to_index_map_type::iterator mapping;
    bool inserted;
    std::tie(mapping, inserted) = m_key_to_index.emplace(
        value.first,
        std::distance(begin(), position)
    );

    if(inserted)
    {
        // New item. Try to insert it
        try
        {
            new_position = m_items.insert(position, value);
        }
        catch(...)
        {
            // Insertion failed. Remove the new
            // mapping to preserve consistency
            m_key_to_index.erase(mapping);
            throw;
        }

        // Increment all the indices after the insertion
        for (auto it = new_position; it != end(); ++it)
            ++(m_key_to_index.find(it->first)->second);
    }
    else
    {
        // Key already exists. Return its position
        new_position = std::next(begin(), mapping->second);
    }

    return std::make_pair(new_position, inserted);
}

template <typename T>
inline
std::pair<typename label_map<T>::iterator, bool>
label_map<T>::insert(const_iterator position, value_type&& value)
{
    // Try to insert the new key
    iterator new_position;
    key_to_index_map_type::iterator mapping;
    bool inserted;
    std::tie(mapping, inserted) = m_key_to_index.emplace(
        value.first,
        std::distance(begin(), position)
    );

    if(inserted)
    {
        // New item. Try to insert it
        try
        {
            new_position = m_items.insert(position, std::move(value));
        }
        catch(...)
        {
            // Insertion failed. Remove the new
            // mapping to preserve consistency
            m_key_to_index.erase(mapping);
            throw;
        }

        // Increment all the indices after the insertion
        for (auto it = new_position; it != end(); ++it)
            ++(m_key_to_index.find(it->first)->second);
    }
    else
    {
        // Key already exists. Return its position
        new_position = std::next(begin(), mapping->second);
    }

    return std::make_pair(new_position, inserted);
}

template <typename T>
template <typename InputIt>
inline
std::pair<typename label_map<T>::iterator, typename label_map<T>::size_type>
label_map<T>::insert(const_iterator position, InputIt first, InputIt last)
{
    iterator new_position = std::next(begin(), std::distance(cbegin(), position));
    size_type count = 0;
    for (auto it = first; it != last; ++it)
    {
        // Try to insert the new key
        key_to_index_map_type::iterator mapping;
        bool inserted;
        std::tie(mapping, inserted) = m_key_to_index.emplace(
            it->first,
            std::distance(begin(), new_position)
        );

        if(inserted)
        {
            // New item. Try to insert it
            try
            {
                new_position = m_items.insert(new_position, std::move(*it));
            }
            catch(...)
            {
                // Insertion failed. Remove the new
                // mapping to preserve consistency
                m_key_to_index.erase(mapping);
                throw;
            }

            // Increment all the indices after the insertion
            for (auto it = new_position; it != end(); ++it)
                ++(m_key_to_index.find(it->first)->second);

            // Increment the insertion count
            ++count;
        }
    }

    return std::make_pair(new_position, count);
}

template <typename T>
inline
std::pair<typename label_map<T>::iterator, typename label_map<T>::size_type>
label_map<T>::insert(const_iterator position, std::initializer_list<value_type> init)
{
    return insert(position, init.begin(), init.end());
}

template <typename T>
template<typename... Args>
inline
std::pair<typename label_map<T>::iterator, bool>
label_map<T>::emplace(const_iterator position, Args&&... args)
{
    insert(position, value_type(std::forward<Args>(args)...));
}

template <typename T>
inline
typename label_map<T>::iterator 
label_map<T>::erase(const_iterator position) noexcept
{
    // Remove the mapping
    m_key_to_index.erase(position->first);

    // Erase from the item list
    const auto result = m_items.erase(position);

    // Decrement all the posterior mappings
    for(auto it = result; it != end(); ++it)
        --(m_key_to_index.find(it->first)->second);
    
    return result;
}

template <typename T>
inline
typename label_map<T>::iterator 
label_map<T>::erase(const_iterator first, const_iterator last) noexcept
{
    // Remove all the mappings
    for (auto it = first; it != last; ++it)
        m_key_to_index.erase(it->first);

    // Erase from the item list
    const auto count = std::distance(first, last);
    const auto result = m_items.erase(first, last);

    // Decrement all the posterior mappings
    for(auto it = result; it != end(); ++it)
        m_key_to_index.find(it->first)->second -= count;

    return result;
}

template <typename T>
inline
bool label_map<T>::push_back(const value_type& value)
{
    // Try to insert the new key
    key_to_index_map_type::iterator mapping;
    bool inserted;
    std::tie(mapping, inserted) = m_key_to_index.emplace(
        value.first,
        m_items.size()
    );

    if(inserted)
    {
        // New item. Try to insert it
        try
        {
            m_items.push_back(value);
        }
        catch(...)
        {
            // Insertion failed. Remove the new
            // mapping to preserve consistency
            m_key_to_index.erase(mapping);
            throw;
        }
    }

    return inserted;
}

template <typename T>
inline
bool label_map<T>::push_back(value_type&& value)
{
    // Try to insert the new key
    key_to_index_map_type::iterator mapping;
    bool inserted;
    std::tie(mapping, inserted) = m_key_to_index.emplace(
        value.first,
        m_items.size()
    );

    if(inserted)
    {
        // New item. Try to insert it
        try
        {
            m_items.push_back(std::move(value));
        }
        catch(...)
        {
            // Insertion failed. Remove the new
            // mapping to preserve consistency
            m_key_to_index.erase(mapping);
            throw;
        }
    }

    return inserted;
}

template <typename T>
template<typename... Args>
inline
bool label_map<T>::emplace_back(Args&&... args)
{
    return push_back(value_type(std::forward<Args>(args)...));
}

template <typename T>
inline
void label_map<T>::pop_back() noexcept
{
    m_key_to_index.erase(m_items.back().first);
    m_items.pop_back();
}



template<typename T>
inline
bool operator==(const label_map<T>& left, const label_map<T>& right) noexcept
{  
    typename label_map<T>::const_iterator last_left, last_right;
    std::tie(last_left, last_right) = std::mismatch(
        left.begin(), left.end(),
        right.begin(), right.end()
    );

    return last_left == left.end() && last_right == right.end();
}

template<typename T>
inline
bool operator!=(const label_map<T>& left, const label_map<T>& right) noexcept
{
    return !(left == right);
}

} // namespace metadata
} // namespace xmipp4
