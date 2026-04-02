// SPDX-License-Identifier: GPL-3.0-only

#include "kernel_cache_key.hpp"

namespace xmipp4 
{
namespace multidimensional
{

inline
bool kernel_cache_key::operator==(const kernel_cache_key &other) const noexcept
{
	return false; // TODO
}

inline
bool kernel_cache_key::operator!=(const kernel_cache_key &other) const noexcept
{
	return !(*this == other);
}

inline
std::size_t kernel_cache_key::hash() const noexcept
{
	return 0; // TODO
}

} // namespace multidimensional
} // namespace xmipp4

namespace std
{

inline
size_t hash<xmipp4::multidimensional::kernel_cache_key>::operator()(
	const xmipp4::multidimensional::kernel_cache_key& key
) const noexcept
{
	return key.hash();
}

} // namespace std
