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
    : m_items(first, last)
{
    auto it = m_items.begin();
    while(it != m_items.end())
    {
        bool inserted;
        std::tie(std::ignore, inserted) = m_key_to_position.emplace(
            it->first, 
            it
        );

        if(inserted)
            ++it;
        else
            it = m_items.erase(it);
    }
}

template <typename T>
inline
label_map<T>::label_map(std::initializer_list<value_type> init)
    : label_map(init.begin(), init.end())
{
}

template <typename T>
inline
label_map<T>::label_map(const label_map& other)
    : m_items(other.m_items)
{
    for(auto it = m_items.begin(); it != m_items.end(); ++it)
        m_key_to_position.emplace(it->first, it);
}

template <typename T>
inline
label_map<T>& label_map<T>::operator=(const label_map& other)
{
    *this = label_map(other);
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
    m_key_to_position.clear();
}

template <typename T>
template <typename Key>
inline
typename label_map<T>::mapped_type& 
label_map<T>::operator[](Key&& key)
{
    typename key_to_position_map_type::iterator mapping;
    bool inserted;
    std::tie(mapping, inserted) = m_key_to_position.insert(
        std::piecewise_construct,
        std::forward_as_tuple<Key>(key),
        std::forward_as_tuple()
    );

    if (inserted)
    {
        // Element did not exist. Insert it 
        // at the end and update the mapping
        try
        {
            mapping->second = m_items.emplace(
                m_items.cend(),
                std::piecewise_construct,
                std::forward_as_tuple(mapping->first),
                std::forward_as_tuple()
            );
        }
        catch (...)
        {
            // Could not insert the new element
            // into de list. Erase the mapping
            // to preserve consistency
            m_key_to_position.erase(mapping);
        }
    }

    return *(mapping->second);
}

template <typename T>
inline
typename label_map<T>::mapped_type& 
label_map<T>::at(const key_type& key)
{
    const auto mapping = m_key_to_position.find(key);
    if(mapping == m_key_to_position.end())
        throw std::out_of_range("Key not present");
    return *(mapping->second);
}

template <typename T>
inline
const typename label_map<T>::mapped_type& 
label_map<T>::at(const key_type& key) const
{
    const auto mapping = m_key_to_position.find(key);
    if(mapping == m_key_to_position.end())
        throw std::out_of_range("Key not present");
    return *(mapping->second);
}

template <typename T>
inline
typename label_map<T>::iterator 
label_map<T>::find(const key_type& key) noexcept
{
    const auto mapping = m_key_to_position.find(key);
    return mapping == m_key_to_position.end() ? end() : mapping->second;
}

template <typename T>
inline
typename label_map<T>::const_iterator 
label_map<T>::find(const key_type& key) const noexcept
{
    const auto mapping = m_key_to_position.find(key);
    return mapping == m_key_to_position.end() ? end() : mapping->second;
}

template <typename T>
inline
bool label_map<T>::contains(const key_type& key) const noexcept
{
    return m_key_to_position.find(key) != m_key_to_position.end();
}

template <typename T>
inline
typename label_map<T>::insertion_result
label_map<T>::insert(const_iterator position, const value_type& value)
{
    return emplace(value);
}

template <typename T>
inline
typename label_map<T>::insertion_result
label_map<T>::insert(const_iterator position, value_type&& value)
{
    return emplace(std::move(value));
}

template <typename T>
template <typename InputIt>
inline
typename label_map<T>::multiple_insertion_result
label_map<T>::insert(const_iterator position, InputIt first, InputIt last)
{
    const auto insert_position = m_items.insert(position, first, last);
    return insert_mapping(insert_position, position);
}

template <typename T>
inline
typename label_map<T>::multiple_insertion_result
label_map<T>::insert(const_iterator position, std::initializer_list<value_type> init)
{
    return insert(position, init.begin(), init.end());
}

template <typename T>
inline
typename label_map<T>::multiple_insertion_result
label_map<T>::splice(const_iterator position, label_map& other)
{
    return splice(position, std::move(other));
}

template <typename T>
inline
typename label_map<T>::multiple_insertion_result
label_map<T>::splice(const_iterator position, label_map&& other)
{
    const auto first = other.begin();
    m_items.splice(position, std::move(other.m_items));
    other.m_key_to_position.clear();
    return insert_mapping(first, position);
}

template <typename T>
inline
typename label_map<T>::insertion_result
label_map<T>::splice(const_iterator position, label_map& other, const_iterator item)
{
    return splice(position, std::move(other), item);
}

template <typename T>
inline
typename label_map<T>::insertion_result
label_map<T>::splice(const_iterator position, label_map&& other, const_iterator item)
{
    m_items.splice(position, std::move(other.m_items), item);
    return insert_mapping(item);
}

template <typename T>
inline
typename label_map<T>::multiple_insertion_result
label_map<T>::splice(const_iterator position, label_map& other, const_iterator first, const_iterator last)
{
    return splice(position, std::move(other), first, last);
}

template <typename T>
inline
typename label_map<T>::multiple_insertion_result
label_map<T>::splice(const_iterator position, label_map&& other, const_iterator first, const_iterator last)
{
    m_items.splice(position, std::move(other.m_items), first, last);
    return insert_mapping(first, position);
}

template <typename T>
template<typename... Args>
inline
typename label_map<T>::insertion_result
label_map<T>::emplace(const_iterator position, Args&&... args)
{
    const auto emplace_position = m_items.emplace(position, std::forward<Args>(args)...);
    return insert_mapping(emplace_position);
}



template <typename T>
inline
typename label_map<T>::iterator 
label_map<T>::erase(const_iterator position) noexcept
{
    m_key_to_position.erase(*position);
    return m_items.erase(position);
}

template <typename T>
inline
bool label_map<T>::erase(const key_type key) noexcept
{
    const auto mapping = m_key_to_position.find(key);
    const auto result = mapping == m_key_to_position.end();
    if(result)
    {
        m_items.erase(mapping->second);
        m_key_to_position.erase(mapping);
    }
    return result;
}

template <typename T>
inline
typename label_map<T>::iterator 
label_map<T>::erase(const_iterator first, const_iterator last) noexcept
{
    for(auto it = first; it != last; ++it)
        m_key_to_position.erase(*it);
    return m_items.erase(first, last);
}

template <typename T>
inline
typename label_map<T>::insertion_result
label_map<T>::push_back(const value_type& value)
{
    return emplace_back(value);
}

template <typename T>
inline
typename label_map<T>::insertion_result
label_map<T>::push_back(value_type&& value)
{
    return emplace_back(std::move(value));
}

template <typename T>
template<typename... Args>
inline
typename label_map<T>::insertion_result
label_map<T>::emplace_back(Args&&... args)
{
    return emplace(cend(), std::forward<Args>(args)...);
}

template <typename T>
inline
typename label_map<T>::insertion_result
label_map<T>::push_front(const value_type& value)
{
    return emplace_front(value);
}

template <typename T>
inline
typename label_map<T>::insertion_result
label_map<T>::push_front(value_type&& value)
{
    return emplace_front(std::move(value));
}

template <typename T>
template<typename... Args>
inline
typename label_map<T>::insertion_result
label_map<T>::emplace_front(Args&&... args)
{
    return emplace(cbegin(), std::forward<Args>(args)...);
}

template <typename T>
inline
void label_map<T>::pop_back() noexcept
{
    m_key_to_position.erase(m_items.back());
    m_items.pop_back();    
}

template <typename T>
inline
void label_map<T>::pop_front() noexcept
{
    m_key_to_position.erase(m_items.front());
    m_items.pop_front();    
}

template <typename T>
inline
void label_map<T>::swap_ordering(const_iterator x, const_iterator y) noexcept
{
    if (x != y)
    {
        // Relink the list so that positions
        // of x and y are swapped
        const auto tmp = std::next(x);
        m_items.splice(y, x);
        m_items.splice(tmp, y);
    }
}

template <typename T>
template <typename Key>
inline
bool label_map<T>::rename(iterator position, Key&& key)
{
    // Try to insert the new key
    typename key_to_position_map_type::iterator mapping;
    bool inserted;
    std::tie(mapping, inserted) = m_key_to_position.insert(
        std::piecewise_construct,
        std::forward_as_tuple<Key>(key),
        std::forward_as_tuple()
    );

    if(inserted)
    {
        try
        {
            // New key does not exist, insert
            // move the old value to a new key
            mapping->second = m_items.emplace(
                position,
                std::piecewise_construct,
                std::forward_as_tuple(mapping->first),
                std::forward_as_tuple(std::move(position->second))
            );
        }
        catch (...)
        {
            // An error ocurred inserting the new
            // item. Remove the mapping to preserve consistency
            m_key_to_position.erase(mapping);
        }

        // Delete the old node and mapping
        m_key_to_position.erase(position->first);
        m_items.erase(position);
    }
    
    return inserted;

}



template <typename T>
inline
typename label_map<T>::insertion_result
label_map<T>::insert_mapping(iterator position)
{
    typename key_to_position_map_type::iterator mapping;
    bool inserted;
    try
    {
        std::tie(mapping, inserted) = m_key_to_position.emplace(
            position->first,
            position
        );
    }
    catch(...)
    {
        // An exception occurred when updating
        // the label map. Erase the inserted item
        // to keep consistency
        m_items.erase(position);
        throw; // Rethrow
    }

    if (!inserted)
    {
        // Key already exists. Erase the inserted item
        // and return the existing item with the desired
        // key
        m_items.erase(position);
        position = mapping->second;
    }

    return std::make_pair(position, inserted);
}

template <typename T>
inline
typename label_map<T>::multiple_insertion_result
label_map<T>::insert_mapping(iterator first, const_iterator last)
{   
    size_type count = 0;

    // Remove all the duplicated elements at the beginning
    // while updating the first position (as it is deleted)
    while(first != last)
    {
        bool inserted;
        try
        {
            std::tie(std::ignore, inserted) = m_key_to_position.emplace(
                first->first, 
                first
            );
        }
        catch(...)
        {
            // An exception occurred when updating
            // the label map. Erase all the unmapped
            // items to keep consistency
            m_items.erase(first, last);
            throw; // Rethrow
        }

        if (inserted)
        {
            // First successful insertion
            count = 1;
            break;
        }
        else
        {
            first = m_items.erase(first);
        }
    }

    // Update the mapping for the rest of elements
    // also keeping track of duplicated elements
    if (first != last)
    {
        auto it = std::next(first);
        while(it != last)
        {
            bool inserted;
            try
            {
                std::tie(std::ignore, inserted) = m_key_to_position.emplace(
                    it->first, 
                    it
                );
            }
            catch(...)
            {
                // An exception occurred when updating
                // the label map. Erase all the unmapped
                // items to keep consistency
                m_items.erase(it, last);
                throw; // Rethrow
            }

            if (inserted)
            {
                ++it;
                ++count;
            }
            else
            {
                it = m_items.erase(it);
            }
        }
    }

    return std::make_pair(first, count);
}



template<typename T>
inline
bool operator==(const label_map<T>& left, const label_map<T>& right) noexcept
{  
    return std::equal(
        left.begin(), left.end(),
        right.begin(), right.end()
    );
}

template<typename T>
inline
bool operator!=(const label_map<T>& left, const label_map<T>& right) noexcept
{
    return !(left == right);
}

} // namespace metadata
} // namespace xmipp4
