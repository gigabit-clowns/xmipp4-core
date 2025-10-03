// SPDX-License-Identifier: GPL-3.0-only

#include "flagset.hpp"

#include "bit.hpp"

namespace xmipp4
{
namespace binary
{

template<typename B>
XMIPP4_INLINE_CONSTEXPR flagset<B>::flagset() noexcept
    : flagset(unsigned_type(0))
{
}

template<typename B>
XMIPP4_INLINE_CONSTEXPR flagset<B>::flagset(bit_type bit) noexcept
    : flagset(static_cast<unsigned_type>(bit))
{
}

template<typename B>
XMIPP4_INLINE_CONSTEXPR flagset<B>::flagset(underlying_type data) noexcept
    : m_data(data)
{
}

template<typename B>
template<typename It>
XMIPP4_INLINE_CONSTEXPR flagset<B>::flagset(It first, It last) noexcept
    : flagset()
{
    while (first != last)
    {
        *this |= *first;
        ++first;
    }
}

template<typename B>
XMIPP4_INLINE_CONSTEXPR flagset<B>::flagset(std::initializer_list<bit_type> bits) noexcept
    : flagset(bits.begin(), bits.end())
{
}






template<typename B>
XMIPP4_INLINE_CONSTEXPR flagset<B>::operator bool() const noexcept
{
    return static_cast<bool>(m_data);
}

template<typename B>
XMIPP4_CONSTEXPR flagset<B>::operator underlying_type() const noexcept
{
    return get_bits();
}

template<typename B>
XMIPP4_INLINE_CONSTEXPR flagset<B>& 
flagset<B>::operator|=(const flagset& rhs) noexcept
{
    m_data |= rhs.m_data;
    return *this;
}

template<typename B>
XMIPP4_INLINE_CONSTEXPR flagset<B>& 
flagset<B>::operator&=(const flagset& rhs) noexcept
{
    m_data &= rhs.m_data;
    return *this;
}

template<typename B>
XMIPP4_INLINE_CONSTEXPR flagset<B>& 
flagset<B>::operator^=(const flagset& rhs) noexcept
{
    m_data ^= rhs.m_data;
    return *this;
}

template<typename B>
XMIPP4_INLINE_CONSTEXPR bool 
flagset<B>::operator==(const flagset& rhs) const noexcept
{
    return m_data == rhs.m_data;
}

template<typename B>
XMIPP4_INLINE_CONSTEXPR bool 
flagset<B>::operator!=(const flagset& rhs) const noexcept
{
    return m_data != rhs.m_data;
}



template<typename B>
XMIPP4_INLINE_CONSTEXPR 
typename flagset<B>::underlying_type
flagset<B>::get_bits() const noexcept
{
    return m_data;
}

template<typename B>
XMIPP4_INLINE_CONSTEXPR bool 
flagset<B>::contains(const bit_type& bit) const noexcept
{
    return any_of(flagset(bit));
}

template<typename B>
XMIPP4_INLINE_CONSTEXPR bool 
flagset<B>::all_of(const flagset& other) const noexcept
{
    return (*this & other) == other;
}

template<typename B>
XMIPP4_INLINE_CONSTEXPR bool 
flagset<B>::any_of(const flagset& other) const noexcept
{
    return static_cast<bool>(*this & other);
}

template<typename B>
XMIPP4_INLINE_CONSTEXPR bool 
flagset<B>::none_of(const flagset& other) const noexcept
{
    return !any_of(other);
}

template<typename B>
XMIPP4_INLINE_CONSTEXPR bool 
flagset<B>::only_of(const flagset& other) const noexcept
{
    return other.all_of(*this);
}

template<typename B>
XMIPP4_INLINE_CONSTEXPR int 
flagset<B>::count() const noexcept
{
    return binary::popcount(m_data);
}

template<typename B>
XMIPP4_INLINE_CONSTEXPR bool
flagset<B>::parity() const noexcept
{
    return binary::parity(m_data);
}

template<typename B>
XMIPP4_INLINE_CONSTEXPR bool
flagset<B>::has_single_bit() const noexcept
{
    return binary::has_single_bit(m_data);
}

template<typename B>
XMIPP4_INLINE_CONSTEXPR flagset<B>& 
flagset<B>::set(const flagset& other, bool value) noexcept
{
    if(value) {
        set(other);
    } else {
        clear(other);
    }
    return *this;
}

template<typename B>
XMIPP4_INLINE_CONSTEXPR flagset<B>& 
flagset<B>::set(const flagset& other) noexcept
{
    return *this |= other;
}

template<typename B>
XMIPP4_INLINE_CONSTEXPR flagset<B>& 
flagset<B>::clear() noexcept
{
    return *this = flagset();
}

template<typename B>
XMIPP4_INLINE_CONSTEXPR flagset<B>& 
flagset<B>::clear(const flagset& other) noexcept
{
    m_data &= ~other.m_data;
    return *this;
}

template<typename B>
XMIPP4_INLINE_CONSTEXPR flagset<B>& 
flagset<B>::toggle(const flagset& other) noexcept
{
    return *this ^= other;
}



template<typename B>
XMIPP4_INLINE_CONSTEXPR 
flagset<B> operator|(const flagset<B>& lhs, const flagset<B>& rhs) noexcept
{
    return flagset<B>(lhs.get_bits() | rhs.get_bits());
}

template<typename B>
XMIPP4_INLINE_CONSTEXPR 
flagset<B> operator&(const flagset<B>& lhs, const flagset<B>& rhs) noexcept
{
    return flagset<B>(lhs.get_bits() & rhs.get_bits());
}

template<typename B>
XMIPP4_INLINE_CONSTEXPR 
flagset<B> operator^(const flagset<B>& lhs, const flagset<B>& rhs) noexcept
{
    return flagset<B>(lhs.get_bits() ^ rhs.get_bits());
}

} // namespace binary
} // namespace xmipp4

template <typename B>
XMIPP4_CONSTEXPR size_t 
std::hash<xmipp4::binary::flagset<B>>::operator()(xmipp4::binary::flagset<B> b) const noexcept
{
    using underlying_type = typename xmipp4::binary::flagset<B>::underlying_type;
    return static_cast<underlying_type>(b);
}