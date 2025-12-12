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

	/**
	 * @brief Obtain an alias for this array.
	 * 
	 * @return array Another array referencing the contents of this array.
	 */
	XMIPP4_CORE_API 
	array share() noexcept;

private:
	class implementation;
	std::shared_ptr<implementation> m_implementation;

	explicit array(std::shared_ptr<implementation> impl) noexcept;
};

} // namespace multidimensional
} // namespace xmipp4
