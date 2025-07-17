// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "platform/constexpr.hpp"
#include "binary/bit.hpp"
#include "binary/flagset.hpp"

#include <ostream>

namespace xmipp4 
{

enum class access_flag_bits {
    read = binary::bit(0),
    write = binary::bit(1),
};

using access_flags = binary::flagset<access_flag_bits>;

inline XMIPP4_CONST_CONSTEXPR access_flags read_only(access_flag_bits::read);
inline XMIPP4_CONST_CONSTEXPR access_flags write_only(access_flag_bits::write);
inline XMIPP4_CONST_CONSTEXPR access_flags read_write({access_flag_bits::read, access_flag_bits::write});

XMIPP4_CONSTEXPR 
const char* to_string(access_flag_bits v) noexcept;

template<typename T>
std::basic_ostream<T>& operator<<(std::basic_ostream<T>& os, access_flag_bits v);

} // namespace xmipp4

#include "access_flags.inl"