// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "array_iterator.hpp"
#include "../span.hpp"
#include "../platform/dynamic_shared_object.h"

#include <memory>

namespace xmipp4 
{
namespace multidimensional
{

class array_access_layout_implementation;

/**
 * @brief Provides joint access semantics for array-s
 * 
 * This class enables jointly iterating over multiple arrays. It stores
 * extents of the iteration space, strides for each of the operands, 
 * and offset for each of the operands.
 * 
 * Instances of this object must be obtained through 
 * array_access_layout_builder. Once built, it is inmutable.
 * 
 * @see array_access_layout_builder
 * 
 */
class array_access_layout
{
public:
	XMIPP4_CORE_API array_access_layout();
	XMIPP4_CORE_API explicit array_access_layout(
		std::unique_ptr<const array_access_layout_implementation> implementation
	);
	array_access_layout(const array_access_layout&) = delete;
	XMIPP4_CORE_API array_access_layout(array_access_layout&& other) noexcept;
	XMIPP4_CORE_API ~array_access_layout();

	array_access_layout& 
	operator=(const array_access_layout&) = delete;
	XMIPP4_CORE_API array_access_layout& 
	operator=(array_access_layout&& other) noexcept;

	/**
	 * @brief Get the number of operands
	 * 
	 * @return std::size_t The number of operands.
	 */
	XMIPP4_CORE_API std::size_t get_number_of_operands() const noexcept;

	/**
	 * @brief Get the number of dimensions.
	 * 
	 * @return std::size_t Number of dimensions.
	 */
	XMIPP4_CORE_API std::size_t get_rank() const noexcept;

	/**
	 * @brief Get the extents of the iteration space.
	 * 
	 * @return span<const std::size_t> The extents of the iteration space.
	 */
	XMIPP4_CORE_API span<const std::size_t> get_extents() const;

	/**
	 * @brief Get the strides for an operand.
	 * 
	 * @param operand The index of the operand. Must be in 
	 * [0, number_of_operands())
	 * @return span<const std::ptrdiff_t> The strides of the requested operand. 
	 * Always has the same size as extents.
	 * 
	 * @throws std::out_of_range if operand index is out of range.
	 */
	XMIPP4_CORE_API
	span<const std::ptrdiff_t> get_strides(std::size_t operand) const;

	/**
	 * @brief Get the offset parameter of an operand.
	 * 
	 * @param operand The index of the operand. Must be in 
	 * [0, number_of_operands())
	 * @return std::ptrdiff_t The offset parameter of the requested operand.
	 * 
	 * @throws std::out_of_range if operand index is out of range.
	 */
	XMIPP4_CORE_API
	std::ptrdiff_t get_offset(std::size_t operand) const;

	/**
	 * @brief Populate an array iterator for traversing this layout.
	 * 
	 * @param ite The iterator to be populated.
	 * @param dim Index of the inner-most dimension to be iterated. Must
	 * be less or equal to get_rank(). Defaults to zero.
	 * @return std::size_t Number of elements in the dimension. 0 if the
	 * layout can not be iterated.
	 */
	XMIPP4_CORE_API
	std::size_t iter(array_iterator &ite, std::size_t dim=0) const;

	/**
	 * @brief Advance an array iterator.
	 * 
	 * @param ite The iterator to be advanced. Must have been populated by 
	 * `iter()`. The previous call to `iter()` or `next()` must have returned
	 * a non-zero value.
	 * @param n Number of elements to be advanced. Must be less or equal to the 
	 * number returned by the previous call to `iter()` or `next()`. Otherwise 
	 * behavior is undefined.
	 * @param dim Index of the inner-most dimension to be iterated. Must
	 * be less or equal to `get_rank()`. Defaults to zero. Although strictly not
	 * mandated, it is recommended to keep this parameter constant across 
	 * iterations. 
	 * @return std::size_t Number of remaining elements in `dim`. 0 if iteration 
	 * has finished.
	 * 
	 * @warning If dim is modified during iteration, the value provided to `n` 
	 * needs special attention as the limit may be smaller than the last value 
	 * returned by `iter()` or `next()`. If unsure, the new limit may be 
	 * computed using `next(ite, 0, dim)`.
	 */
	XMIPP4_CORE_API
	std::size_t next(
		array_iterator &ite, 
		std::size_t n, 
		std::size_t dim = 0
	) const noexcept;

	/**
	 * @brief Get a pointer to the implementation.
	 * 
	 * @return const array_access_layout_implementation* Pointer to the
	 * implementation.
	 * 
	 * @note This method is meant to be used for testing purposes, as the 
	 * array_access_layout_implementation is not publicly available.
	 */
	const array_access_layout_implementation* 
	get_implementation() const noexcept;

private:
	std::unique_ptr<const array_access_layout_implementation> m_implementation;
};

} // namespace multidimensional
} // namespace xmipp4
