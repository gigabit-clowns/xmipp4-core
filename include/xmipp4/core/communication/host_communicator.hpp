// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "communicator.hpp"
#include "reduction_operation.hpp"
#include "../numerical_type.hpp"
#include "../platform/dynamic_shared_object.h"

#include <memory>
#include <cstddef>

namespace xmipp4 
{
namespace communication
{

class host_operation;

/**
 * @brief Abstract class to represent interprocess and inter-node 
 * communications.
 * 
 */
class XMIPP4_CORE_API host_communicator
	: public communicator
{
public:
	host_communicator() = default;
	host_communicator(const host_communicator &other) = delete;
	host_communicator(host_communicator &&other) = delete;
	~host_communicator() override = default;

	host_communicator& operator=(const host_communicator &other) = delete;
	host_communicator& operator=(host_communicator &&other) = delete;

	/**
	 * @brief Split the current communicator.
	 * 
	 * Split a communicator into multiple communicators containing.
	 * 
	 * @param colour The group where the current rank will be assigned to.
	 * @param rank_priority Hint to assign the rank in the new communicator.
	 * @return std::shared_ptr<host_communicator> The communicator where
	 * the current rank has been assigned to.
	 * 
	 */
	virtual std::shared_ptr<host_communicator> split(
		int colour, 
		int rank_priority
	) const = 0;

	/**
	 * @brief Create a barrier operation.
	 * 
	 * @return std::shared_ptr<host_operation> The barrier operation.
	 */
	virtual std::shared_ptr<host_operation> create_barrier() = 0;

	/**
	 * @brief Create a send operation.
	 * 
	 * @param data Pointer to the data to be sent.
	 * @param data_type Type of the elements.
	 * @param data_count Number of elements.
	 * @param destination_rank Rank of the destination.
	 * @return std::shared_ptr<host_operation> The send operation.
	 */
	virtual std::shared_ptr<host_operation> create_send(
		const void *data, 
		numerical_type data_type, 
		std::size_t data_count,
		int destination_rank
	) = 0;

	/**
	 * @brief Create a receive operation.
	 * 
	 * @param data Pointer to the data to be received.
	 * @param data_type Type of the elements.
	 * @param data_count Number of elements.
	 * @param source_rank Rank of the source.
	 * @return std::shared_ptr<host_operation> The receive operation.
	 */
	virtual std::shared_ptr<host_operation> create_receive(
		void *data, 
		numerical_type data_type, 
		std::size_t data_count,
		int source_rank
	) = 0;

	/**
	 * @brief Create a broadcast operation.
	 * 
	 * @param data Pointer to the data where the broadcast happens.
	 * @param data_type Type of the elements.
	 * @param data_count Number of elements.
	 * @param root_rank Rank of the root.
	 * @return std::shared_ptr<host_operation> The broadcast operation.
	 */
	virtual std::shared_ptr<host_operation> create_broadcast(
		void *data, 
		numerical_type data_type, 
		std::size_t data_count,
		int root_rank
	) = 0;

	/**
	 * @brief Create a reduce operation.
	 * 
	 * @param data Pointer to the data to be reduced.
	 * @param data_type Type of the elements.
	 * @param data_count Number of elements.
	 * @param reduction Type of the reduction operation.
	 * @param root_rank Rank of the root.
	 * @return std::shared_ptr<host_operation> The reduce operation.
	 */
	virtual std::shared_ptr<host_operation> create_reduce(
		void *data, 
		numerical_type data_type, 
		std::size_t data_count,
		reduction_operation reduction,
		int root_rank
	) = 0;

	/**
	 * @brief Create an all_reduce operation.
	 * 
	 * @param data Pointer to the data to be reduced.
	 * @param data_type Type of the elements.
	 * @param data_count Number of elements.
	 * @param reduction Type of the reduction operation.
	 * @return std::shared_ptr<host_operation> The all_reduce operation.
	 */
	virtual std::shared_ptr<host_operation> create_all_reduce(
		void *data, 
		numerical_type data_type, 
		std::size_t data_count,
		reduction_operation reduction
	) = 0;

};

} // namespace communication
} // namespace xmipp4
