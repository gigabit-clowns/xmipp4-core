// SPDX-License-Identifier: GPL-3.0-only

#include "typed_operation_program_cache_key.hpp"

#include <utility>
#include <functional>

namespace xmipp4
{
namespace multidimensional
{

template <typename K>
inline
typed_operation_program_cache_key<K>::typed_operation_program_cache_key(K key)
	: m_key(std::move(key))
{
}

template <typename K>
inline std::size_t
typed_operation_program_cache_key<K>::hash() const noexcept
{
	return std::hash<K>{}(m_key);
}

template <typename K>
inline bool
typed_operation_program_cache_key<K>::equals(
	const operation_program_cache_key &other
) const noexcept
{
	// Precondition: the cache guarantees that other has the same
	// dynamic type as *this, so a static_cast is sound.
	return static_cast<const typed_operation_program_cache_key&>(other)
		.m_key == m_key;
}

template <typename K>
inline const K&
typed_operation_program_cache_key<K>::get_key() const noexcept
{
	return m_key;
}

} // namespace multidimensional
} // namespace xmipp4
