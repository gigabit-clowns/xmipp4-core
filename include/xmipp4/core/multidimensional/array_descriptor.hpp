// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "strided_layout.hpp"
#include "../numerical_type.hpp"
#include "../platform/dynamic_shared_object.h"

namespace xmipp4 
{
namespace multidimensional
{

/**
 * @brief Description of a multidimensional array.
 * 
 * This class represents the (strided_layout, numerical_type) pair that
 * defines the data interpretation on a multidimensional array.
 * 
 * @see array
 * 
 */
class array_descriptor
{
public:
	/**
	 * @brief Default construct the descriptor with an empty layout and 
	 * unknown numerical type.
	 * 
	 */
	XMIPP4_CORE_API
	array_descriptor() noexcept;

	/**
	 * @brief Construct a descriptor from its components.
	 * 
	 * @param layout The layout.
	 * @param data_type The numerical type.
	 */
	XMIPP4_CORE_API
	array_descriptor(strided_layout layout, numerical_type data_type) noexcept;

	XMIPP4_CORE_API
	array_descriptor(const array_descriptor &other);
	XMIPP4_CORE_API
	array_descriptor(array_descriptor &&other) noexcept;
	XMIPP4_CORE_API
	~array_descriptor();

	XMIPP4_CORE_API
	array_descriptor& operator=(const array_descriptor &other);
	XMIPP4_CORE_API
	array_descriptor& operator=(array_descriptor &&other) noexcept;

	/**
	 * @brief Get the layout of the array.
	 * 
	 * @return const strided_layout& The layout.
	 */
	XMIPP4_CORE_API
	const strided_layout& get_layout() const noexcept;

	/**
	 * @brief Get the data type of the array.
	 * 
	 * @return numerical_type The numerical type.
	 */
	XMIPP4_CORE_API
	numerical_type get_data_type() const noexcept;

private:
	strided_layout m_layout;
	numerical_type m_data_type;
};

XMIPP4_CORE_API
bool operator==(
	const array_descriptor &lhs, 
	const array_descriptor &rhs
) noexcept;

XMIPP4_CORE_API
bool operator!=(
	const array_descriptor &lhs, 
	const array_descriptor &rhs
) noexcept;

/**
 * @brief Compute the number of bytes required to store the given descriptor.
 * 
 * @param descriptor The descriptor from which the storage requirement is 
 * computed.
 * @return std::size_t Minimum number of bytes required to store the descriptor.
 */
XMIPP4_CORE_API
std::size_t compute_storage_requirement(const array_descriptor &descriptor);

} // namespace multidimensional
} // namespace xmipp4
