// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "../span.hpp"
#include "../platform/dynamic_shared_object.h"

#include <vector>

namespace xmipp4 
{
namespace layout
{

/**
 * @brief State of a multidimensional multi-array traversal.
 */
class access_iterator
{
public:
	/**
	 * @brief Default constructor.
	 */
	XMIPP4_CORE_API
	access_iterator() noexcept;

	/**
	 * @brief Construct an array iterator at its initial state.
	 * 
	 * @param ndim Number of dimensions. Initial indices will correspond to 
	 * this amount of zeros.
	 * @param offsets The base offsets for each operand.
	 */
	XMIPP4_CORE_API
	access_iterator(
		std::size_t ndim,
		std::vector<std::ptrdiff_t> offsets
	);

	XMIPP4_CORE_API
	access_iterator(const access_iterator &other);
	XMIPP4_CORE_API
	access_iterator(access_iterator &&other) noexcept;
	XMIPP4_CORE_API
	~access_iterator();

	XMIPP4_CORE_API
	access_iterator& operator=(const access_iterator &other);
	XMIPP4_CORE_API
	access_iterator& operator=(access_iterator &&other) noexcept;

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
	 * @return span<const std::ptrdiff_t> The offsets.
	 */
	XMIPP4_CORE_API
	span<const std::ptrdiff_t> get_offsets() const noexcept;

	/**
	 * @brief Get the current offsets for each operand.
	 * 
	 * Unless assigned to another iterator, the pointed span is guaranteed to 
	 * remain constant.
	 * 
	 * @return span<std::ptrdiff_t> The offsets.
	 */
	XMIPP4_CORE_API
	span<std::ptrdiff_t> get_offsets() noexcept;

private:
	std::vector<std::size_t> m_indices;
	std::vector<std::ptrdiff_t> m_offsets;
};

} // namespace layout
} // namespace xmipp4
