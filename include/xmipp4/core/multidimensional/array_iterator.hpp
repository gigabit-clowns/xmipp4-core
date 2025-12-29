// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "../span.hpp"
#include "../platform/dynamic_shared_object.h"

#include <vector>

namespace xmipp4 
{
namespace multidimensional
{

/**
 * @brief State of a multidimensional multi-array traversal.
 */
class array_iterator
{
public:
	/**
	 * @brief Default constructor.
	 */
	XMIPP4_CORE_API
	array_iterator() noexcept;

	/**
	 * @brief Construct an array iterator at its initial state.
	 * 
	 * @param ndim Number of dimensions. Initial indices will correspond to 
	 * this amount of zeros.
	 * @param offsets The base offsets for each operand.
	 */
	XMIPP4_CORE_API
	array_iterator(
		std::size_t ndim,
		std::vector<std::size_t> offsets
	);

	XMIPP4_CORE_API
	array_iterator(const array_iterator &other);
	XMIPP4_CORE_API
	array_iterator(array_iterator &&other) noexcept;
	XMIPP4_CORE_API
	~array_iterator();

	XMIPP4_CORE_API
	array_iterator& operator=(const array_iterator &other);
	XMIPP4_CORE_API
	array_iterator& operator=(array_iterator &&other) noexcept;

	/**
	 * @brief Get the current multidimensional indices.
	 * 
	 * Unless assigned to another iterator, the pointed span is guaranteed to 
	 * remain constant.
	 * 
	 * @return span<const std::size_t> The indices.
	 */
	XMIPP4_CORE_API
	span<const std::size_t> get_indices() const noexcept;

	/**
	 * @brief Get the current multidimensional indices.
	 * 
	 * Unless assigned to another iterator, the pointed span is guaranteed to 
	 * remain constant.
	 * 
	 * @return span<std::size_t> The indices.
	 */
	XMIPP4_CORE_API
	span<std::size_t> get_indices() noexcept;

	/**
	 * @brief Get the current offsets for each operand.
	 * 
	 * Unless assigned to another iterator, the pointed span is guaranteed to 
	 * remain constant.
	 * 
	 * @return span<const std::size_t> The offsets.
	 */
	XMIPP4_CORE_API
	span<const std::size_t> get_offsets() const noexcept;

	/**
	 * @brief Get the current offsets for each operand.
	 * 
	 * Unless assigned to another iterator, the pointed span is guaranteed to 
	 * remain constant.
	 * 
	 * @return span<std::size_t> The offsets.
	 */
	XMIPP4_CORE_API
	span<std::size_t> get_offsets() noexcept;

private:
	std::vector<std::size_t> m_indices;
	std::vector<std::size_t> m_offsets;
};

} // namespace multidimensional
} // namespace xmipp4
