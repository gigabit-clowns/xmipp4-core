// SPDX-License-Identifier: GPL-3.0-only

#include "array_access_layout_operand.hpp"

#include <xmipp4/core/platform/assert.hpp>

#include <numeric>
#include <algorithm>
#include <sstream>

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

inline
array_access_layout_implementation::array_access_layout_implementation(
	const extent_vector_type &extents
)
	: m_extents(extents)
{
}

inline
void array_access_layout_implementation::add_operand(
	stride_vector_type strides,
	std::ptrdiff_t offset
)
{
	XMIPP4_ASSERT( m_extents.size() == strides.size() );
	m_operands.emplace_back(
		std::move(strides),
		offset
	);
}

inline
const array_access_layout_operand& 
array_access_layout_implementation::get_operand(std::size_t index) const
{
	if (index >= m_operands.size())
	{
		throw std::out_of_range("Operand index is out of range");
	}

	return m_operands[index];
}

inline
void array_access_layout_implementation::insert_largest_stride(
	span<std::size_t> permutation,
	std::size_t i
)
{
	const auto index1 = i;
	for (std::size_t j = 1; j <= i; ++j)
	{
		const auto index0 = i - j;
		const auto comparison = compare_strides(
			permutation[index0], 
			permutation[index1]
		);

		if (comparison > 0)
		{
			std::swap(permutation[index0], permutation[index1]);
			j = 0;
		} 
		else if (comparison < 0) 
		{
			break;
		}
	}
}

inline
void array_access_layout_implementation::sort_axes_by_locality()
{
	const auto n = m_extents.size();
	if (n <= 1)
	{
		return; // Trivial
	}

	// Start with reversed indices n-1, n-2 ... 1, 0
	boost::container::small_vector<
		std::size_t, XMIPP4_SMALL_AXIS_COUNT
	> permutation(n);
	const auto permutation_view = span<std::size_t>(permutation.data(), n);
	std::generate(
		permutation.begin(), 
		permutation.end(), 
		[n = n]() mutable { return --n; }
	);

	// Insertion sort with support for ambiguous comparisons
	for (std::size_t i = 1; i < n; ++i)
	{
		insert_largest_stride(permutation_view, i);
	}

	permute_axes(permutation_view);
}

inline
void array_access_layout_implementation::coalesce_contiguous_axes()
{
	const auto n = m_extents.size();
	if (n <= 1)
	{
		return; // Trivial
	}

	std::size_t prev = 0;
	for (std::size_t curr = 1; curr < n;  ++curr)
	{
		if (!try_coalesce_axes(prev, curr))
		{
			++prev;
			if(prev != curr) // Avoid unnecessary swaps.
			{
				swap_axes(prev, curr);
			}
		}
	}

	trim_axes(prev+1);
}

inline
std::size_t 
array_access_layout_implementation::get_number_of_operands() const noexcept
{
	return m_operands.size();
}

inline
span<const std::size_t> 
array_access_layout_implementation::get_extents() const noexcept
{
	return span<const std::size_t>(m_extents.data(), m_extents.size());
}

inline
span<const std::ptrdiff_t> 
array_access_layout_implementation::get_strides(std::size_t operand) const
{
	return get_operand(operand).get_strides();
}

inline
std::ptrdiff_t 
array_access_layout_implementation::get_offset(std::size_t operand) const
{
	return get_operand(operand).get_offset();
}

inline
std::size_t array_access_layout_implementation::iter(array_iterator &ite) const
{
	const auto valid = std::all_of(
		m_extents.cbegin(), 
		m_extents.cend(),
		[] (auto extent)
		{
			return extent > 0;
		}
	);

	if (!valid)
	{
		// There is at least 1 empty axis
		return 0UL;
	}

	std::vector<std::size_t> offsets;
	offsets.reserve(m_operands.size());
	std::transform(
		m_operands.cbegin(),
		m_operands.cend(),
		std::back_inserter(offsets),
		std::mem_fn(&array_access_layout_operand::get_offset)
	);

	ite =  array_iterator(
		m_extents.size(),
		std::move(offsets)
	);

	if (m_extents.empty())
	{
		return 1UL;
	}

	return m_extents.front();
}

inline
std::size_t array_access_layout_implementation::next(
	array_iterator &ite,
	std::size_t n
) const noexcept
{
	const auto extents = get_extents();
	const auto n_dim = extents.size();
	const auto n_operands = get_number_of_operands();

	const auto indices = ite.get_indices();
	const auto offsets = ite.get_offsets();
	XMIPP4_ASSERT( indices.size() == n_dim );
	XMIPP4_ASSERT( offsets.size() == n_operands );

	if (n_dim > 0 && n != 1)
	{
		const auto block_increment = n - 1;
		indices[0] += block_increment;
		for (std::size_t j = 0; j < n_operands; ++j) 
		{
			const auto strides = m_operands[j].get_strides();
			offsets[j] += block_increment*strides[0];
		}
	}

	for (std::size_t i = 0; i < n_dim; ++i) 
	{
		const auto next_index = indices[i] + 1;
		const auto extent = extents[i];

		if (next_index < extent)
		{
			for (std::size_t j = 0; j < n_operands; ++j) 
			{
				const auto strides = m_operands[j].get_strides();
				offsets[j] += strides[i];
			}

			indices[i] = next_index;

			return extents[0] - indices[0];
		}

		for (std::size_t j = 0; j < n_operands; ++j)
		{
			const auto strides = m_operands[j].get_strides();
			offsets[j] -= (indices[i] * strides[i]);
		}

		indices[i] = 0;
	}
	
	return 0UL; 
}

inline
int array_access_layout_implementation::compare_strides(
	std::size_t i, 
	std::size_t j
) noexcept
{
	for (const auto &operand : m_operands)
	{
		const auto cmp = operand.compare_strides(i, j);
		if (cmp != 0)
		{
			return cmp;
		}
	}

	// Untie with extents
	return static_cast<int>(m_extents[i]) - static_cast<int>(m_extents[j]); 
}

inline
void array_access_layout_implementation::swap_axes(
	std::size_t i, 
	std::size_t j
) noexcept
{
	std::swap(m_extents[i], m_extents[j]);
	for (auto &operand : m_operands)
	{
		operand.swap_axes(i, j);
	}
}

inline
void array_access_layout_implementation::permute_axes(
	span<std::size_t> permutation
)
{
	// Permute the extents using cycle decomposition
	const auto n = permutation.size();
	for (size_t i = 0; i < n; ++i) {
		while (permutation[i] != i) {
			swap_axes(i, permutation[i]);
			std::swap(permutation[i], permutation[permutation[i]]);
		}
	}
}

inline
bool array_access_layout_implementation::try_coalesce_axes(
	std::size_t i, 
	std::size_t j
)
{
	if (m_extents[i] == 1)
	{
		swap_axes(i, j);
		return true;
	}

	if (can_coalesce_axes(i, j))
	{
		m_extents[i] *= m_extents[j];
		return true;
	}

	return false;
}

inline
bool array_access_layout_implementation::can_coalesce_axes(
	std::size_t i, 
	std::size_t j
)
{
	const auto extent_i = m_extents[i];
	const auto extent_j = m_extents[j];
	if (extent_j == 1)
	{
		return true; // Trivial, as any stride_j is meaningless.
	}

	for (const auto &operand : m_operands)
	{
		const auto strides = operand.get_strides();
		const auto stride_i = strides[i];
		const auto stride_j = strides[j];
		if (static_cast<std::ptrdiff_t>(extent_i) * stride_i != stride_j) 
		{
			return false;
		}
	}

	return true;
}

inline
void array_access_layout_implementation::trim_axes(std::size_t n)
{
	m_extents.resize(n);
	for (auto &operand : m_operands)
	{
		operand.trim_axes(n);
	}
}

} // namespace multidimensional
} // namespace xmipp4
