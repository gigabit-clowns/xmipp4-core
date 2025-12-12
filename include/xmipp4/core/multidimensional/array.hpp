// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "array_descriptor.hpp"
#include "dynamic_subscript.hpp"
#include "strided_layout.hpp"
#include "../numerical_type.hpp"
#include "../span.hpp"
#include "../platform/attributes.hpp"
#include "../platform/dynamic_shared_object.h"

#include <array>
#include <memory>

namespace xmipp4 
{
namespace hardware
{

class device_queue;
class buffer;

} // namespace hardware
namespace multidimensional
{

class array_descriptor;
class strided_layout;

class array
{
public:
	XMIPP4_CORE_API array();
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
	 * @brief Set the descriptor for this array.
	 *
	 * @param descriptor The descriptor to be set.
	 * 
	 * @warning This function does not check if the current storage is suitable
	 * for this descriptor.
	 *  
	 */
	XMIPP4_CORE_API void set_descriptor(array_descriptor descriptor);

	/**
	 * @brief Get the descriptor of this array.
	 * 
	 * @return array_descriptor The descriptor.
	 */
	XMIPP4_CORE_API const array_descriptor& get_descriptor() const noexcept;

	/**
	 * @brief Set the storage of this array.
	 * 
	 * @param storage A share pointer to the storage to be adopted.
	 * 
	 * @warning This function does not check if the storage is suitable for the
	 * current descriptor.
	 * 
	 */
	XMIPP4_CORE_API void set_storage(std::shared_ptr<hardware::buffer> storage);

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
	 * @brief Get the storage of this array.
	 * 
	 * @return std::shared_ptr<const hardware::buffer> The storage. nullptr if 
	 * the array is not initialized
	 */
	XMIPP4_CORE_API 
	std::shared_ptr<const hardware::buffer> share_storage() const noexcept;
	
	/**
	 * @brief Get a shared pointer to the storage of this array.
	 * 
	 * @return std::shared_ptr<hardware::buffer> The storage. nullptr if the 
	 * array is not initialized.
	 */
	XMIPP4_CORE_API 
	std::shared_ptr<hardware::buffer> share_storage() noexcept;

private:
	class implementation;
	std::shared_ptr<implementation> m_implementation;

	explicit array(std::shared_ptr<implementation> impl) noexcept;
	explicit array(implementation &&impl);
};

} // namespace multidimensional
} // namespace xmipp4
