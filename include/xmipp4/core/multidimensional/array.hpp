// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "array_descriptor.hpp"
#include "array_view.hpp"
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

class array_implementation;

/**
 * @brief Container for multidimensional numeric data.
 * 
 * This class allows to represent multidimensional numeric data in a hardware
 * agnostic manner. It serves as a foundational basis to define interfaces that
 * operate in such date. The array is represented as the (storage, descriptor)
 * pair. The storage refers to an untyped region of memory (which may reside in 
 * accelerator hardware like GPUs). The descriptor defines the interpretation of
 * that data to give raise to the notion of dimensions.
 */
class array
{
public:
	/**
	 * @brief Construct an uninitialized array.
	 */
	XMIPP4_CORE_API array();

	/**
	 * @brief Construct an array from its components.
	 * 
	 * @param storage Buffer used to store the data contained in the array.
	 * @param descriptor The interpretation of the buffer memory.
	 */
	XMIPP4_CORE_API array(
		std::shared_ptr<hardware::buffer> storage,
		array_descriptor descriptor
	);

	array(const array& other) = delete;
	XMIPP4_CORE_API array(array&& other) noexcept;
	XMIPP4_CORE_API ~array();

	array& operator=(const array& other) = delete;
	XMIPP4_CORE_API array& operator=(array&& other) noexcept;

	/**
	 * @brief Get the descriptor of this array.
	 * 
	 * @return array_descriptor The descriptor.
	 */
	XMIPP4_CORE_API const array_descriptor& get_descriptor() const noexcept;

	/**
	 * @brief Get the storage of this array.
	 * 
	 * @return hardware::buffer* The storage. nullptr if the array is not 
	 * initialized.
	 */
	XMIPP4_CORE_API hardware::buffer* get_storage() noexcept;

	/**
	 * @brief Get the storage of this array.
	 * 
	 * @return const hardware::buffer* The storage. nullptr if the array is not
	 * initialized
	 */
	XMIPP4_CORE_API const hardware::buffer* get_storage() const noexcept;
	
	/**
	 * @brief Get a shared pointer to the storage of this array.
	 * 
	 * @return std::shared_ptr<hardware::buffer> The storage. nullptr if the 
	 * array is not initialized.
	 */
	XMIPP4_CORE_API 
	std::shared_ptr<hardware::buffer> share_storage() noexcept;
	
	/**
	 * @brief Get the storage of this array.
	 * 
	 * @return std::shared_ptr<const hardware::buffer> The storage. nullptr if 
	 * the array is not initialized
	 */
	XMIPP4_CORE_API 
	std::shared_ptr<const hardware::buffer> share_storage() const noexcept;

	/**
	 * @brief Obtain an alias for this array.
	 * 
	 * @return array Another array referencing the contents of this array.
	 */
	XMIPP4_CORE_API 
	array share() noexcept;

	/**
	 * @brief Obtain a read-only alias for this array
	 * 
	 * @return array An array_view referencing the contents of this array.
	 */
	XMIPP4_CORE_API 
	array_view share() const noexcept;

private:
	std::shared_ptr<const array_implementation> m_implementation;

	explicit array(
		std::shared_ptr<const array_implementation> implementation
	) noexcept;
};

} // namespace multidimensional
} // namespace xmipp4
