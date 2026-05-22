// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "operation_command_cache_key.hpp"

#include <xmipp4/core/platform/dynamic_shared_object.h>

#include <cstddef>
#include <functional>
#include <memory>
#include <typeindex>

namespace xmipp4
{
namespace multidimensional
{

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
	 * Lookup is partitioned by the dynamic type of @p key, so entries
	 * stored under keys of a different dynamic type are never considered.
	 * On a hit, the entry is promoted to the lowest eviction priority, so
	 * recently touched entries are the last to be evicted.
	 *
	 * @param key The key to look up. Its dynamic type and value must match
	 * those of the key under which the entry was stored.
	 * @return std::shared_ptr<void> The stored value, or nullptr if no
	 * entry is present.
	 */
	XMIPP4_CORE_API
	std::shared_ptr<void> touch(const operation_command_cache_key &key);

	/**
	 * @brief Store a value under the given key, replacing any existing
	 * entry under a key of the same dynamic type and value. May evict
	 * another entry when the cache is at capacity.
	 *
	 * The newly stored or replaced entry is placed at the lowest eviction
	 * priority.
	 *
	 * @param key The key under which the value is stored. The cache takes
	 * ownership. Must not be null.
	 * @param value The value to store. Must not be null.
	 * @throws std::invalid_argument When @p key or @p value is null.
	 */
	XMIPP4_CORE_API
	void store(
		std::unique_ptr<const operation_command_cache_key> key,
		std::shared_ptr<void> value
	);

private:
	class implementation;
	std::unique_ptr<implementation> m_implementation;
};

} // namespace multidimensional
} // namespace xmipp4
