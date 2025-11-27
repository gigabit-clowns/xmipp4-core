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
 * @brief Represents an equally type and sized pair of a dynamically typed host 
 * memory region.
 * 
 */
class host_send_receive_regions
{
public:
	/**
	 * @brief Construct an empty region pair.
	 *
	 */
	XMIPP4_CONSTEXPR
	host_send_receive_regions() noexcept;

	/**
	 * @brief Construct the regions from their respective typed pointers and 
	 * count.
	 * 
	 * @tparam T Type of the elements.
	 * @param send_data Read only pointer to the data to be sent.
	 * @param receive_data Pointer to the data to be received.
	 * @param count Number of elements.
	 */
	template <typename T>
	XMIPP4_CONSTEXPR
	host_send_receive_regions(
		const T *send_data, 
		T *receive_data, 
		std::size_t count
	) noexcept;

	/**
	 * @brief Construct the regions from their respective untyped pointers, 
	 * dynamic type and count.
	 * 
	 * @param send_data Read only pointer to the data to be sent.
	 * @param receive_data Pointer to the data to be received.
	 * @param data_type Type of the elements pointed by the pointers.
	 * @param count Number of elements.
	 */
	XMIPP4_CONSTEXPR
	host_send_receive_regions(
		const void *send_data, 
		void *receive_data, 
		numerical_type data_type, 
		std::size_t count
	) noexcept;

	/**
	 * @brief Construct an aliasing pair of regions from a typed pointer.
	 * 
	 * @tparam T Type of the elements.
	 * @param send_receive_data Pointer to the data to be sent and received.
	 * @param count Number of elements.
	 */
	template <typename T>
	XMIPP4_CONSTEXPR
	host_send_receive_regions(T *send_recv_data, std::size_t count) noexcept;

	/**
	 * @brief Construct an aliasing pair of regions from a untyped pointer and
	 * dynamic type and count.
	 * 
	 * @param send_receive_data Pointer to the data to be sent and received.
	 * @param data_type Type of the elements pointed by the pointers.
	 * @param count Number of elements.
	 */
	XMIPP4_CONSTEXPR
	host_send_receive_regions(
		void *send_recv_data, 
		numerical_type data_type, 
		std::size_t count
	) noexcept;

	host_send_receive_regions(const host_send_receive_regions &other) = default;
	host_send_receive_regions(host_send_receive_regions &&other) = default;
	~host_send_receive_regions() = default;

	host_send_receive_regions& 
	operator=(const host_send_receive_regions &other) = default;
	host_send_receive_regions& 
	operator=(host_send_receive_regions &&other) = default;

	/**
	 * @brief Get an untype pointer to the data to be sent.
	 * 
	 * @return const void* Pointer to the data to be sent.
	 */
	XMIPP4_CONSTEXPR const void* get_send_data() const noexcept;

	/**
	 * @brief Get an untype pointer to the data to be received.
	 * 
	 * @return void* Pointer to the data to be received.
	 */
	XMIPP4_CONSTEXPR void* get_receive_data() const noexcept;

	/**
	 * @brief Get the data type.
	 * 
	 * @return numerical_type The data type.
	 */
	XMIPP4_CONSTEXPR numerical_type get_data_type() const noexcept;

	/**
	 * @brief Get the number of elements referenced by the send and receive
	 * regions.
	 * 
	 * @return std::size_t The element count.
	 */
	XMIPP4_CONSTEXPR std::size_t get_count() const noexcept;

private:
	const void *m_send_data;
	void *m_receive_data;
	numerical_type m_data_type;
	std::size_t m_count;

};

} // namespace communication
} // namespace xmipp4

#include "host_send_receive_regions.inl"
