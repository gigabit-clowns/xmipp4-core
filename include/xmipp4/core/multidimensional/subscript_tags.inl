// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "subscript_tags.hpp"

namespace xmipp4 
{
namespace multidimensional
{

XMIPP4_INLINE_CONSTEXPR
ellipsis_tag ellipsis() noexcept
{
	return ellipsis_tag();
}

XMIPP4_INLINE_CONSTEXPR
bool operator==(ellipsis_tag, ellipsis_tag) noexcept
{
	return true;
}

XMIPP4_INLINE_CONSTEXPR
bool operator!=(ellipsis_tag, ellipsis_tag) noexcept
{
	return false;
}

template <typename T>
inline
std::basic_ostream<T>& operator<<(std::basic_ostream<T> &os, ellipsis_tag)
{
	return os << "ellipsis_tag";
}

XMIPP4_INLINE_CONSTEXPR
new_axis_tag new_axis() noexcept
{
	return new_axis_tag();
}

XMIPP4_INLINE_CONSTEXPR
bool operator==(new_axis_tag, new_axis_tag) noexcept
{
	return true;
}

XMIPP4_INLINE_CONSTEXPR
bool operator!=(new_axis_tag, new_axis_tag) noexcept
{
	return false;
}

template <typename T>
inline
std::basic_ostream<T>& operator<<(std::basic_ostream<T> &os, new_axis_tag)
{
	return os << "new_axis_tag";
}

} // namespace multidimensional
} // namespace xmipp4
