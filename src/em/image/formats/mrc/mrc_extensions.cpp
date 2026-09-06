// SPDX-License-Identifier: GPL-3.0-only

#include "mrc_extensions.hpp"

#include <algorithm>
#include <array>

namespace rexlib
{
namespace em
{
namespace mrc
{

namespace
{

template <std::size_t N>
bool contains(
	const std::array<const char*, N> &extensions,
	const std::string &extension
) noexcept
{
	return std::any_of(
		extensions.cbegin(),
		extensions.cend(),
		[&extension] (const char *candidate)
		{
			return extension == candidate;
		}
	);
}

} // anonymous namespace

bool is_readable_extension(const std::string &extension) noexcept
{
	// .st, .rec and .ali are what IMOD names a tilt series, a reconstruction
	// and an aligned stack, none of which differ from an MRC file in
	// anything but their name.
	static const std::array<const char*, 6> extensions = {{
		".mrc", ".mrcs", ".map", ".st", ".rec", ".ali"
	}};

	return contains(extensions, extension);
}

bool is_writable_extension(const std::string &extension) noexcept
{
	static const std::array<const char*, 3> extensions = {{
		".mrc", ".mrcs", ".map"
	}};

	return contains(extensions, extension);
}

} // namespace mrc
} // namespace em
} // namespace rexlib
