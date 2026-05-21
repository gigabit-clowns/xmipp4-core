// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "../platform/dynamic_shared_object.h"

#include <cstddef>
#include <functional>
#include <memory>
#include <typeindex>

namespace xmipp4
{
namespace multidimensional
{

/**
 * @brief Abstract base for keys stored in an operation_command_cache.
 *
 * The cache partitions its entries by the dynamic type of the key, so each
 * subclass only needs to define hashing and equality among instances of
 * itself. Builders may use the provided typed_operation_command_cache_key
 * wrapper for any key type that is hashable through std::hash and
 * equality-comparable, or subclass this interface directly when finer
 * control is required.
 */
class operation_command_cache_key
{
public:
	XMIPP4_CORE_API
	operation_command_cache_key() noexcept;
	operation_command_cache_key(
		const operation_command_cache_key &other
	) = default;
	operation_command_cache_key(
		operation_command_cache_key &&other
	) = default;
	XMIPP4_CORE_API
	virtual ~operation_command_cache_key();

	operation_command_cache_key&
	operator=(const operation_command_cache_key &other) = default;
	operation_command_cache_key&
	operator=(operation_command_cache_key &&other) = default;

	/**
	 * @brief Hash of the wrapped key.
	 *
	 * @return std::size_t The hash value.
	 */
	virtual std::size_t hash() const noexcept = 0;

	/**
	 * @brief Equality against another key.
	 *
	 * The cache guarantees that this method is only invoked with keys of
	 * the same dynamic type. Implementations may rely on that precondition
	 * and skip any runtime type check.
	 *
	 * @param other The key to compare against.
	 * @return true Both keys refer to the same entry.
	 * @return false The keys differ in value.
	 */
	virtual bool
	equals(const operation_command_cache_key &other) const noexcept = 0;
};

/**
 * @brief Default key wrapper for value types that model std::hash and
 * equality comparison.
 *
 * @tparam K The wrapped key type. Must be hashable through std::hash,
 * equality-comparable, and copy-constructible.
 */
template <typename K>
class typed_operation_command_cache_key final
	: public operation_command_cache_key
{
public:
	explicit typed_operation_command_cache_key(K key);
	typed_operation_command_cache_key(
		const typed_operation_command_cache_key &other
	) = default;
	typed_operation_command_cache_key(
		typed_operation_command_cache_key &&other
	) = default;
	~typed_operation_command_cache_key() override = default;

	typed_operation_command_cache_key&
	operator=(const typed_operation_command_cache_key &other) = default;
	typed_operation_command_cache_key&
	operator=(typed_operation_command_cache_key &&other) = default;

	std::size_t hash() const noexcept override;
	bool equals(
		const operation_command_cache_key &other
	) const noexcept override;

	const K& get_key() const noexcept;

private:
	K m_key;
};

/**
 * @brief Bounded associative store for backend-private resources reused
 * across operation command builds.
 *
 * The cache behaves as a typed key/value bag: each builder defines its own
 * key type and stores its own backend-specific resources (FFT plans,
 * compiled kernels, ...). Entries are partitioned by the static type of
 * the key, so unrelated builders never collide even if their key layouts
 * happen to coincide.
 *
 * The cache holds at most a fixed number of entries set at construction;
 * once that bound is reached, inserting a new entry evicts the oldest one.
 * Eviction order is shared across all key types: a hot inner bucket may
 * evict entries from a cold one.
 */
class operation_command_cache
{
public:
	/**
	 * @brief Construct a cache with the given capacity.
	 *
	 * @param capacity Maximum number of entries the cache may hold. Must
	 * be greater than zero.
	 */
	XMIPP4_CORE_API
	explicit operation_command_cache(std::size_t capacity) noexcept;
	operation_command_cache(const operation_command_cache &other) = delete;
	operation_command_cache(operation_command_cache &&other) = delete;
	XMIPP4_CORE_API
	~operation_command_cache();

	operation_command_cache&
	operator=(const operation_command_cache &other) = delete;
	operation_command_cache&
	operator=(operation_command_cache &&other) = delete;

	/**
	 * @brief Get the maximum number of entries this cache may hold.
	 *
	 * @return std::size_t The capacity, in entries.
	 */
	XMIPP4_CORE_API
	std::size_t get_capacity() const noexcept;

	/**
	 * @brief Look up a previously stored value and grant it amnesty.
	 *
	 * On a hit, the entry is promoted to the lowest eviction priority, so
	 * recently touched entries are the last to be evicted.
	 *
	 * @tparam V Stored value type. Must be specified explicitly.
	 * @tparam K Key type. Deduced from @p key.
	 * @param key The key to look up.
	 * @return std::shared_ptr<V> The stored value, or nullptr if no entry
	 * is present.
	 */
	template <typename V, typename K>
	std::shared_ptr<V> touch(const K &key);

	/**
	 * @brief Store a value under the given key, replacing any existing
	 * entry under the same key. May evict another entry when the cache is
	 * at capacity.
	 *
	 * @tparam K Key type.
	 * @tparam V Stored value type.
	 * @param key The key under which the value is stored.
	 * @param value The value to store.
	 */
	template <typename K, typename V>
	void store(K &&key, std::shared_ptr<V> value);

private:
	class implementation;
	std::unique_ptr<implementation> m_implementation;

	XMIPP4_CORE_API
	std::shared_ptr<void> touch_erased(
		std::type_index type,
		const operation_command_cache_key &key
	);

	XMIPP4_CORE_API
	void store_erased(
		std::type_index type,
		std::unique_ptr<operation_command_cache_key> key,
		std::shared_ptr<void> value
	);
};

} // namespace multidimensional
} // namespace xmipp4

#include "operation_command_cache.inl"
