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

/**
 * @file flagset.hpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Provides flagset class
 * @date 2023-08-09
 * 
 */

#include "../platform/attributes.hpp"
#include "../platform/constexpr.hpp"

#include <initializer_list>
#include <type_traits>
#include <functional>

namespace xmipp4
{
namespace utils
{

/**
 * @brief Stores a set of bits in the form of flags
 * Similar to std::bitset but instead of a index based
 * interface it provides a enum based interface.
 * 
 * @tparam B enum type which contains the declaration of
 * the flag bits
 */
template<typename B>
class flagset {
public:
    /**
     * @brief The provided enum declaration of the flag bits
     * 
     */
    using bit_type = B;

    /**
     * @brief The underlying type of the provided enum
     * 
     */
    using underlying_type = typename std::underlying_type<bit_type>::type;
    
    /**
     * @brief Unsigned underlying type of the provided enum
     * 
     */
    using unsigned_type = typename std::make_unsigned<underlying_type>::type;

    /**
     * @brief Empty constructor. It initializes the set as none
     * of the flags were set
     * 
     */
    XMIPP4_CONSTEXPR flagset() noexcept;

    /**
     * @brief Initializes the set with a single bit set
     * 
     * @param bit The bit to be set
     *
     */
    XMIPP4_CONSTEXPR flagset(bit_type bit) noexcept;

    /**
     * @brief Constructs the flagset from raw binary data
     * 
     * @param data The binary data
     */
    explicit XMIPP4_CONSTEXPR flagset(underlying_type data) noexcept;

    /**
     * @brief Initializes the set with the elements inside the
     * a iterator range
     * 
     * @tparam It forward iterator
     * @param first Iterator to the first element
     * @param last Iterator to the past the end element
     * 
     */
    template<typename It>
    XMIPP4_CONSTEXPR flagset(It first, It last) noexcept;

    /**
     * @brief Initializes the set with a initializer list
     * 
     * @param bits Initializer list with the flags to be set
     * 
     */
    XMIPP4_CONSTEXPR flagset(std::initializer_list<bit_type> bits) noexcept;
    
    /**
     * @brief Copy constructor
     * 
     * @param other The set to be copied from
     */
    flagset(const flagset& other) = default;

    /**
     * @brief Move constructor. Same behaviour as copy constructor
     * 
     * @param other The set to be copied from
     */
    flagset(flagset&& other) = default;

    /**
     * @brief Destroy the flagset object
     * 
     */
    ~flagset() = default;

    /**
     * @brief Copy assign operator
     * 
     * @param other The set to be copied from
     * @return flagset& *this
     */
    flagset& operator=(const flagset& other) = default;
    
    /**
     * @brief Move assign operator. Same behaviour as copy constructor
     * 
     * @param other The set to be copied from
     * @return flagset& *this
     */
    flagset& operator=(flagset&& other) = default;


    /**
     * @brief Checks if the set is nonzero
     * 
     * @return true If there is one or more bits set
     * @return false If there are no bits set
     */
    XMIPP4_CONSTEXPR operator bool() const noexcept;

    /**
     * @brief Converts to the underlying type of the bit definition
     * bits set
     * 
     * @return underlying_type
     */
    explicit XMIPP4_CONSTEXPR operator underlying_type() const noexcept;



    /**
     * @brief Union with another flagset
     * 
     * @param rhs The other flagset
     * @return flagset The result of the union
     */
    XMIPP4_CONSTEXPR flagset operator|(const flagset& rhs) const noexcept;

    /**
     * @brief Intersection with another flagset
     * 
     * @param rhs The other flagset
     * @return flagset The result of the intersection
     */
    XMIPP4_CONSTEXPR flagset operator&(const flagset& rhs) const noexcept;

    /**
     * @brief XOR with another flagset
     * 
     * 
     * @param rhs The other flagset
     * @return flagset The result of the XOR
     */
    XMIPP4_CONSTEXPR flagset operator^(const flagset& rhs) const noexcept;

    /**
     * @brief In-place union with another flagset
     * 
     * @param rhs The other flagset
     * @return flagset& *this 
     */
    XMIPP4_CONSTEXPR flagset& operator|=(const flagset& rhs) noexcept;

    /**
     * @brief In-place intersection with another flagset
     * 
     * @param rhs The other flagset
     * @return flagset& *this 
     */
    XMIPP4_CONSTEXPR flagset& operator&=(const flagset& rhs) noexcept;
    
    /**
     * @brief In-place XOR with another flagset
     * 
     * @param rhs The other flagset
     * @return flagset& *this 
     */
    XMIPP4_CONSTEXPR flagset& operator^=(const flagset& rhs) noexcept;

    /**
     * @brief Compares for equality with another flagset
     * 
     * @param rhs The other flagset
     * @return true If both flagset-s are equal
     * @return false If both flagset-s are not equal
     */
    XMIPP4_CONSTEXPR bool operator==(const flagset& rhs) const noexcept;

    /**
     * @brief Compares for inequality with another flagset
     * 
     * @param rhs The other flagset
     * @return true If both flagset-s are not equal
     * @return false If both flagset-s are equal
     */
    XMIPP4_CONSTEXPR bool operator!=(const flagset& rhs) const noexcept;
    


    /**
     * @brief Checks if all of a set of flags are present in this
     * 
     * @param other The set of flags that needs to be present here
     * @return true If all the flags are present
     * @return false If all the flags are not present
     */
    XMIPP4_CONSTEXPR bool all_of(const flagset& other) const noexcept;
    
    /**
     * @brief Checks if any of a set of flags are present in this
     * 
     * @param other The set of flags that needs to be present here
     * @return true If any the flags are present
     * @return false If any the flags are not present
     */
    XMIPP4_CONSTEXPR bool any_of(const flagset& other) const noexcept;
    
    /**
     * @brief Checks if none of a set of flags are present in this
     * 
     * @param other The set of flags that needs to be absent here
     * @return true If none the flags are present
     * @return false If any the flags are is present
     */
    XMIPP4_CONSTEXPR bool none_of(const flagset& other) const noexcept;
    
    /**
     * @brief Checks if only a set of flags are present in this
     * 
     * @param other The set of allowed flags
     * @return true If only the allowed flags are present
     * @return false If any of the disallowed flags is present
     */
    XMIPP4_CONSTEXPR bool only_of(const flagset& other) const noexcept;

    /**
     * @brief Checks if a particular flag is present
     * 
     * @param bit The flag to be tested
     * @return true if the flag is present
     * @return false if the flag is absent
     */
    XMIPP4_CONSTEXPR bool test(bit_type bit) const noexcept;

    /**
     * @brief Counts the number of flags that are present
     * 
     * @return int The number of flags present here
     */
    XMIPP4_CONSTEXPR int count() const noexcept;

    /**
     * @brief Computes the parity of the stored flags
     * 
     * @return true when the number of flags is odd
     * @return false when the number of flags is even
     */
    XMIPP4_CONSTEXPR bool parity() const noexcept;

    /**
     * @brief Checks if only one flag is present
     * 
     * @return true if only one flag is present
     * @return false if zero or more than one flags are present
     */
    XMIPP4_CONSTEXPR bool has_single_bit() const noexcept;

    /**
     * @brief Set a particular set of flags to the desired value
     * 
     * @param other The set of flags to be changed
     * @param value The value of the flags
     * @return flagset& *this 
     */
    XMIPP4_CONSTEXPR flagset& set(const flagset& other, bool value) noexcept;

    /**
     * @brief Sets a particular set of flags
     * 
     * @param other The set of flags to be set
     * @return flagset& *this 
     */
    XMIPP4_CONSTEXPR flagset& set(const flagset& other) noexcept;

    /**
     * @brief Clears all flags
     * 
     * @return flagset& *this 
     */
    XMIPP4_CONSTEXPR flagset& clear() noexcept;
    
    /**
     * @brief Clears a particular set of flags
     * 
     * @param other The set of flags to be cleared
     * @return flagset& *this 
     */
    XMIPP4_CONSTEXPR flagset& clear(const flagset& other) noexcept;
    
    /**
     * @brief Toggles a particular set of flags
     * 
     * @param other The set of flags to be toggled
     * @return flagset& *this 
     */
    XMIPP4_CONSTEXPR flagset& toggle(const flagset& other) noexcept;

private:
    unsigned_type m_data;

};

} // namespace utils
} // namespace xmipp4

template <typename B>
struct std::hash<xmipp4::utils::flagset<B>>
{
    XMIPP4_CONSTEXPR size_t operator()(xmipp4::utils::flagset<B> b) const noexcept;
};

#include "flagset.inl"
