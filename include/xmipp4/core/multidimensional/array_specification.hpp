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
 * @brief Logical representation of an array without instantiating one.
 * 
 * This class represents the in-memory representation of the array data
 * (`array_descriptor`) and its logical placement in memory (pointer to a
 * `hardware::memory_resource`)
 * 
 * @see array
 * @see array_descriptor
 * @see hardware::memory_resource
 */
class array_specification
{
public:
	XMIPP4_CORE_API
	array_specification() noexcept;

	XMIPP4_CORE_API
	explicit array_specification(
		const array_descriptor &descriptor,
		const hardware::memory_resource* resource = nullptr
	) noexcept;

	XMIPP4_CORE_API
	array_specification(
		const strided_layout& layout,
		numerical_type data_type,
		const hardware::memory_resource* resource = nullptr
	) noexcept;

	XMIPP4_CORE_API
	array_specification(
		strided_layout&& layout,
		numerical_type data_type,
		const hardware::memory_resource* resource = nullptr
	) noexcept;

	XMIPP4_CORE_API
	array_specification(const array_specification &other);
	XMIPP4_CORE_API
	array_specification(array_specification &&other) noexcept;
	XMIPP4_CORE_API
	~array_specification();

	XMIPP4_CORE_API
	array_specification& operator=(const array_specification &other);
	XMIPP4_CORE_API
	array_specification& operator=(array_specification &&other) noexcept;

	XMIPP4_CORE_API
	const strided_layout& get_layout() const noexcept;

	XMIPP4_CORE_API
	numerical_type get_data_type() const noexcept;

	XMIPP4_CORE_API
	const array_descriptor& get_descriptor() const noexcept;

	XMIPP4_CORE_API
	void set_descriptor(const array_descriptor &descriptor) noexcept;

	XMIPP4_CORE_API
	const hardware::memory_resource* get_memory_resource() const noexcept;

	XMIPP4_CORE_API
	void set_memory_resource(
		const hardware::memory_resource* resource
	) noexcept;

	XMIPP4_CORE_API
	static array_specification from_array(const array &a) noexcept;

	XMIPP4_CORE_API
	static array_specification from_array(const array_view &a) noexcept;
private:
	array_descriptor m_descriptor;
	const hardware::memory_resource* m_memory_region;
};

XMIPP4_CORE_API
bool operator==(
	const array_specification &lhs, 
	const array_specification &rhs
) noexcept;

XMIPP4_CORE_API
bool operator!=(
	const array_specification &lhs, 
	const array_specification &rhs
) noexcept;

} // namespace multidimensional
} // namespace xmipp4
