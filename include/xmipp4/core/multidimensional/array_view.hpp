// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "../platform/dynamic_shared_object.h"

#include <memory>

namespace xmipp4 
{
namespace hardware
{

class buffer;

} // namespace hardware

namespace multidimensional
{

class array_descriptor;
class array_implementation;

/**
 * @brief Read-only view into an array.
 * 
 * An array_view shares ownership of the implementation with another array,
 * but it only allows read-only semantics with it.
 * 
 * @see array
 */
class array_view
{
public:	
	/**
	 * @brief Construct an uninitialized array view.
	 */
	XMIPP4_CORE_API array_view();

	/**
	 * @brief Construct an array view from an implementation.
	 * 
	 * @param implementation The implementation to be owned by this view.
	 */
	XMIPP4_CORE_API
	explicit array_view(
		std::shared_ptr<const array_implementation> implementation
	) noexcept;

	array_view(const array_view& other) = delete;
	XMIPP4_CORE_API array_view(array_view&& other) noexcept;
	XMIPP4_CORE_API ~array_view();

	array_view& operator=(const array_view& other) = delete;
	XMIPP4_CORE_API array_view& operator=(array_view&& other) noexcept;

	/**
	 * @brief Get the descriptor of this array.
	 * 
	 * @return array_descriptor The descriptor.
	 */
	XMIPP4_CORE_API const array_descriptor& get_descriptor() const noexcept;

	/**
	 * @brief Get the storage of this array.
	 * 
	 * @return const hardware::buffer* The storage. nullptr if the array is not
	 * initialized
	 */
	XMIPP4_CORE_API const hardware::buffer* get_storage() const noexcept;
	
	/**
	 * @brief Get the storage of this array_view.
	 * 
	 * @return std::shared_ptr<const hardware::buffer> The storage. nullptr if 
	 * the array_view is not initialized
	 */
	XMIPP4_CORE_API 
	std::shared_ptr<const hardware::buffer> share_storage() const noexcept;

	/**
	 * @brief Obtain an alias for this array.
	 * 
	 * @return array_view Another array_view referencing the contents of this 
	 * array.
	 */
	XMIPP4_CORE_API 
	array_view share() const noexcept;

private:
	std::shared_ptr<const array_implementation> m_implementation;
};

} // namespace multidimensional
} // namespace xmipp4
