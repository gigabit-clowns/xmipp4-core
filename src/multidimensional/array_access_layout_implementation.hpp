// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "array_access_layout_operand.hpp"

#include "../config.hpp"

#include <boost/container/small_vector.hpp>

/**
 * Some of the algorithms and data structured featured in this code are based 
 * on:
 * https://github.com/pytorch/pytorch/blob/main/aten/src/ATen/TensorIterator.cpp
 * 
 */

namespace xmipp4 
{
namespace multidimensional
{

class array_access_layout_implementation
{
public:    
	using extent_vector_type = boost::container::small_vector<
		std::size_t, 
		XMIPP4_SMALL_AXIS_COUNT
	>;
	using operand_vector_type = boost::container::small_vector<
		array_access_layout_operand, 
		XMIPP4_SMALL_OPERAND_COUNT
	>;
	using stride_vector_type = array_access_layout_operand::stride_vector_type;

	array_access_layout_implementation() = default;
	explicit array_access_layout_implementation(
		const extent_vector_type &extents
	);

	void add_operand(
		stride_vector_type strides,
		std::ptrdiff_t offset
	);

	const array_access_layout_operand& get_operand(std::size_t index) const ;

	void sort_axes_by_locality();

	void coalesce_contiguous_axes();

	std::size_t get_number_of_operands() const noexcept;

	span<const std::size_t> get_extents() const noexcept;
	
	span<const std::ptrdiff_t> get_strides(std::size_t operand) const;

	std::ptrdiff_t get_offset(std::size_t operand) const;

	std::size_t iter(array_iterator &ite) const;

	std::size_t next(array_iterator &ite, std::size_t n) const noexcept;

private:
	extent_vector_type m_extents;
	operand_vector_type m_operands;

	void insert_largest_stride(
		span<std::size_t> permutation,
		std::size_t i
	);

	int compare_strides(std::size_t i, std::size_t j) noexcept;

	void swap_axes(std::size_t i, std::size_t j) noexcept;

	void permute_axes(span<std::size_t> permutation);

	bool try_coalesce_axes(std::size_t i, std::size_t j);

	bool can_coalesce_axes(std::size_t i, std::size_t j);

	void trim_axes(std::size_t n);

	void apply_strides(
		span<std::size_t> offsets, 
		std::size_t position, 
		std::ptrdiff_t multiplier
	) const noexcept;
};

} // namespace multidimensional
} // namespace xmipp4

#include "array_access_layout_implementation.inl"
