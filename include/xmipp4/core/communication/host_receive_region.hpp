// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "../numerical_type.hpp"
#include "../platform/constexpr.hpp"

#include <cstddef>

namespace xmipp4 
{
namespace communication
{

/**
 * @brief Representation of a dynamically typed host memory region.
 * 
 */
class host_receive_region
{
public:
	/**
	 * @brief Construct an empty region.
	 *
	 */
	XMIPP4_CONSTEXPR
	host_receive_region() noexcept;

	/**
	 * @brief Construct a memory region from a typed pointer and element count.
	 * 
	 * @tparam T Type of the elements.
	 * @param data Pointer to the elements.
	 * @param count Number of elements.
	 */
	template <typename T>
	XMIPP4_CONSTEXPR
	host_receive_region(T* data, std::size_t count) noexcept;

	/**
	 * @brief Construct a memory region from a untyped pointer, dynamic type and
	 * element count
	 * 
	 * @param data Untyped pointer.
	 * @param data_type Type of the elements pointed by the pointer.
	 * @param count Number of elements.
	 */
	XMIPP4_CONSTEXPR
	host_receive_region(
		void *data, 
		numerical_type data_type, 
		std::size_t count
	) noexcept;

	host_receive_region(const host_receive_region &other) = default;
	host_receive_region(host_receive_region &&other) = default;
	~host_receive_region() = default;

	host_receive_region& operator=(const host_receive_region &other) = default;
	host_receive_region& operator=(host_receive_region &&other) = default;

	/**
	 * @brief Get an untyped pointer to the data.
	 * 
	 * @return void* the pointer.
	 */
	XMIPP4_CONSTEXPR void* get_data() const noexcept;

	/**
	 * @brief Get the type of the elements.
	 * 
	 * @return numerical_type the type.
	 */
	XMIPP4_CONSTEXPR numerical_type get_data_type() const noexcept;

	/**
	 * @brief Get the number of elements.
	 * 
	 * @return std::size_t element count.
	 */
	XMIPP4_CONSTEXPR std::size_t get_count() const noexcept;

private:
	void *m_data;
	numerical_type m_data_type;
	std::size_t m_count;
};

} // namespace communication
} // namespace xmipp4

#include "host_receive_region.inl"
