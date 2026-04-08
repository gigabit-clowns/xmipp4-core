// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "array_descriptor.hpp"

#include <xmipp4/core/hardware/memory_resource.hpp>
#include <xmipp4/core/platform/dynamic_shared_object.h>

namespace xmipp4 
{
namespace multidimensional
{

class array;
class array_view;

/**
 * @brief Logical representation of an array properties.
 * 
 * This class contains the memory representation of the array data
 * (`array_descriptor`) and its placement in memory (pointer to a
 * `hardware::memory_resource`)
 * 
 * @see array
 * @see array_descriptor
 * @see hardware::memory_resource
 */
class array_signature
{
public:
	XMIPP4_CORE_API
	array_signature() noexcept;

	XMIPP4_CORE_API
	explicit array_signature(
		const array_descriptor &descriptor,
		const hardware::memory_resource* resource = nullptr
	) noexcept;

	XMIPP4_CORE_API
	array_signature(
		const strided_layout& layout,
		numerical_type data_type,
		const hardware::memory_resource* resource = nullptr
	) noexcept;

	XMIPP4_CORE_API
	array_signature(
		strided_layout&& layout,
		numerical_type data_type,
		const hardware::memory_resource* resource = nullptr
	) noexcept;

	XMIPP4_CORE_API
	array_signature(const array_signature &other);
	XMIPP4_CORE_API
	array_signature(array_signature &&other) noexcept;
	XMIPP4_CORE_API
	~array_signature();

	XMIPP4_CORE_API
	array_signature& operator=(const array_signature &other);
	XMIPP4_CORE_API
	array_signature& operator=(array_signature &&other) noexcept;

	XMIPP4_CORE_API
	bool operator==(const array_signature &other) const noexcept;

	XMIPP4_CORE_API
	bool operator!=(const array_signature &other) const noexcept;

	/**
	 * @brief Get the hash value for this object.
	 * 
	 * @return The hash value.
	 */
	XMIPP4_CORE_API
	std::size_t hash() const noexcept;

	/**
	 * @brief Get the in memory layout of the data.
	 * 
	 * @return const strided_layout& The layout.
	 */
	XMIPP4_CORE_API
	const strided_layout& get_layout() const noexcept;

	/**
	 * @brief Get the data type of the elements.
	 * 
	 * @return numerical_type The data type.
	 */
	XMIPP4_CORE_API
	numerical_type get_data_type() const noexcept;

	/**
	 * @brief Get the array descriptor.
	 * 
	 * @return const array_descriptor& The array descriptor.
	 */
	XMIPP4_CORE_API
	const array_descriptor& get_descriptor() const noexcept;

	/**
	 * @brief Set the array descriptor.
	 * 
	 * @param descriptor The new array descriptor.
	 */
	XMIPP4_CORE_API
	void set_descriptor(const array_descriptor &descriptor) noexcept;

	/**
	 * @brief Get the memory resource.
	 * 
	 * @return const hardware::memory_resource* The memory resource.
	 */
	XMIPP4_CORE_API
	const hardware::memory_resource* get_memory_resource() const noexcept;

	/**
	 * @brief Set the memory resource.
	 * 
	 * @param resource The new memory resource.
	 */
	XMIPP4_CORE_API
	void set_memory_resource(
		const hardware::memory_resource* resource
	) noexcept;

	/**
	 * @brief Construct an array signature from an existing array.
	 * 
	 * @param a The array from which the signature is deduced.
	 * @return array_signature The newly constructed array signature.
	 */
	XMIPP4_CORE_API
	static array_signature from_array(const array &a) noexcept;

	/**
	 * @brief Construct an array signature from an existing array_view.
	 * 
	 * @param a The array from which the signature is deduced.
	 * @return array_signature The newly constructed array signature.
	 */
	XMIPP4_CORE_API
	static array_signature from_array(const array_view &a) noexcept;

private:
	array_descriptor m_descriptor;
	const hardware::memory_resource* m_memory_region;
};

} // namespace multidimensional
} // namespace xmipp4
