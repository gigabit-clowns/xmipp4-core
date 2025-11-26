// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "communicator.hpp"
#include "../platform/dynamic_shared_object.h"

#include <memory>
#include <cstddef>

namespace xmipp4 
{
namespace hardware
{

class device;
class device_queue;
class buffer;

}

namespace communication
{

class device_operation;

/**
 * @brief Abstract class to represent inter device communications.
 * 
 */
class XMIPP4_CORE_API device_communicator
	: public communicator
{
public:
	device_communicator() = default;
	device_communicator(const device_communicator &other) = delete;
	device_communicator(device_communicator &&other) = delete;
	~device_communicator() override = default;

	device_communicator& operator=(const device_communicator &other) = delete;
	device_communicator& operator=(device_communicator &&other) = delete;

	/**
	 * @brief Get the device associated to this communicator.
	 * 
	 * @return hardware::device& The device associated to this communicator.
	 */
	virtual hardware::device& get_device() const noexcept = 0;

	/**
	 * @brief Split the current communicator.
	 * 
	 * Split a communicator into multiple communicators containing.
	 * 
	 * @param colour The group where the current rank will be assigned to.
	 * @param rank_priority Hint to assign the rank in the new communicator.
	 * @return std::shared_ptr<device_communicator> The communicator where
	 * the current rank has been assigned to.
	 * 
	 */
	virtual std::shared_ptr<device_communicator> split(
		int colour, 
		int rank_priority
	) const = 0;

	/**
	 * @brief Create a send operation.
	 * 
	 * @param data Buffer with the data to be sent.
	 * @param data_type Type of the elements.
	 * @param data_count Number of elements.
	 * @param destination_rank Rank of the destination.
	 * @return std::shared_ptr<device_operation> The send operation.
	 */
	virtual std::shared_ptr<device_operation> create_send(
		std::shared_ptr<const hardware::buffer> data,
		numerical_type data_type, 
		std::size_t data_count,
		int destination_rank
	) = 0;

	/**
	 * @brief Create a receive operation.
	 * 
	 * @param data Buffer with the data to be received.
	 * @param data_type Type of the elements.
	 * @param data_count Number of elements.
	 * @param source_rank Rank of the source.
	 * @return std::shared_ptr<device_operation> The receive operation.
	 */
	virtual std::shared_ptr<device_operation> create_receive(
		std::shared_ptr<hardware::buffer> data,
		numerical_type data_type, 
		std::size_t data_count,
		int source_rank
	) = 0;

	/**
	 * @brief Create a broadcast operation.
	 * 
	 * @param data Buffer where the broadcast is performed.
	 * @param data_type Type of the elements.
	 * @param data_count Number of elements.
	 * @param root_rank Rank of the root.
	 * @return std::shared_ptr<device_operation> The broadcast operation.
	 */
	virtual std::shared_ptr<device_operation> create_broadcast(
		std::shared_ptr<hardware::buffer> data,
		numerical_type data_type, 
		std::size_t data_count,
		int root_rank
	) = 0;

	/**
	 * @brief Create a reduce operation.
	 * 
	 * @param data Buffer where the reduction is performed.
	 * @param data_type Type of the elements.
	 * @param data_count Number of elements.
	 * @param reduction Type of the reduction operation.
	 * @param root_rank Rank of the root.
	 * @return std::shared_ptr<device_operation> The reduce operation.
	 */
	virtual std::shared_ptr<device_operation> create_reduce(
		std::shared_ptr<hardware::buffer> data,
		numerical_type data_type, 
		std::size_t data_count,
		reduction_operation reduction,
		int root_rank
	) = 0;

	/**
	 * @brief Create an all_reduce operation.
	 * 
	 * @param data Buffer where the reduction is performed.
	 * @param data_type Type of the elements.
	 * @param data_count Number of elements.
	 * @param reduction Type of the reduction operation.
	 * @return std::shared_ptr<device_operation> The all_reduce operation.
	 */
	virtual std::shared_ptr<device_operation> create_all_reduce(
		void *data, 
		numerical_type data_type, 
		std::size_t data_count,
		reduction_operation reduction
	) = 0;

};

} // namespace communication
} // namespace xmipp4
