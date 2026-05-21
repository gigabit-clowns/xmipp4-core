// SPDX-License-Identifier: GPL-3.0-only

#include "operation_command_cache.hpp"

#include <utility>

namespace xmipp4
{
namespace multidimensional
{

template <typename K>
inline
typed_operation_command_cache_key<K>::typed_operation_command_cache_key(K key)
	: m_key(std::move(key))
{
}

template <typename K>
inline std::size_t
typed_operation_command_cache_key<K>::hash() const noexcept
{
	return std::hash<K>{}(m_key);
}

template <typename K>
inline bool
typed_operation_command_cache_key<K>::equals(
	const operation_command_cache_key &other
) const noexcept
{
	// Precondition: the cache guarantees that @p other has the same
	// dynamic type as *this, so a static_cast is sound.
	return static_cast<const typed_operation_command_cache_key&>(other)
		.m_key == m_key;
}

template <typename K>
inline const K&
typed_operation_command_cache_key<K>::get_key() const noexcept
{
	return m_key;
}

template <typename V, typename K>
inline std::shared_ptr<V>
operation_command_cache::touch(const K &key)
{
	const typed_operation_command_cache_key<K> wrapped(key);
	return std::static_pointer_cast<V>(
		touch_erased(typeid(K), wrapped)
	);
}

template <typename K, typename V>
inline void
operation_command_cache::store(K &&key, std::shared_ptr<V> value)
{
	using key_type = typename std::decay<K>::type;
	store_erased(
		typeid(key_type),
		std::make_unique<typed_operation_command_cache_key<key_type>>(
			std::forward<K>(key)
		),
		std::move(value)
	);
}

} // namespace multidimensional
} // namespace xmipp4
