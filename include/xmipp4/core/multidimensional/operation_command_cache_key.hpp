// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/platform/dynamic_shared_object.h>

#include <cstddef>

namespace xmipp4
{
namespace multidimensional
{

/**
 * @brief Abstract base for keys stored in an @ref operation_command_cache.
 */
class XMIPP4_CORE_API operation_command_cache_key
{
public:
	operation_command_cache_key() noexcept;
	operation_command_cache_key(
		const operation_command_cache_key &other
	) = delete;
	operation_command_cache_key(
		operation_command_cache_key &&other
	) = delete;
	virtual ~operation_command_cache_key();

	operation_command_cache_key&
	operator=(const operation_command_cache_key &other) = delete;
	operation_command_cache_key&
	operator=(operation_command_cache_key &&other) = delete;

	/**
	 * @brief Hash of the wrapped key.
	 *
	 * @return std::size_t The hash value.
	 */
	virtual std::size_t hash() const noexcept = 0;

	/**
	 * @brief Equality against another key.
	 *
	 * The cache guarantees that this method is only invoked with keys of the
	 * same type. Implementations may rely on that precondition and skip any 
	 * runtime type check.
	 *
	 * @param other The key to compare against.
	 * @return true Both keys refer to the same entry.
	 * @return false The keys differ in value.
	 */
	virtual 
	bool equals(const operation_command_cache_key &other) const noexcept = 0;
};

} // namespace multidimensional
} // namespace xmipp4
