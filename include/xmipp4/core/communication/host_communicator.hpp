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
class send_buffer;
class receive_buffer;
class send_receive_buffer;

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
	 * @param buffer Buffer to be sent. Its contents must be valid during the 
	 * lifetime of the operation.
	 * @param destination_rank Rank of the destination.
	 * @return std::shared_ptr<host_operation> The send operation.
	 */
	virtual std::shared_ptr<host_operation> create_send(
		const send_buffer &buffer,
		int destination_rank
	) = 0;

	/**
	 * @brief Create a receive operation.
	 * 
	 * @param buffer Buffer where the received contents are written. Its 
	 * contents must be valid during the lifetime of the operation.
	 * @param source_rank Rank of the source.
	 * @return std::shared_ptr<host_operation> The receive operation.
	 */
	virtual std::shared_ptr<host_operation> create_receive(
		const receive_buffer &buffer,
		int source_rank
	) = 0;

	/**
	 * @brief Create a broadcast operation.
	 * 
	 * @param buffer Buffer where the broadcast happens.
	 * @param root_rank Rank of the root.
	 * @return std::shared_ptr<host_operation> The broadcast operation.
	 */
	virtual std::shared_ptr<host_operation> create_broadcast(
		const send_receive_buffer &buffer,
		int root_rank
	) = 0;

	/**
	 * @brief Create a reduce operation.
	 * 
	 * @param buffer Buffer where the the reduction happens.
	 * @param reduction Type of the reduction operation.
	 * @param root_rank Rank of the root.
	 * @return std::shared_ptr<host_operation> The reduce operation.
	 */
	virtual std::shared_ptr<host_operation> create_reduce(
		const send_receive_buffer &buffer,
		reduction_operation reduction,
		int root_rank
	) = 0;

	/**
	 * @brief Create an all_reduce operation.
	 * 
	 * @param buffer Buffer where the the reduction happens.
	 * @param reduction Type of the reduction operation.
	 * @return std::shared_ptr<host_operation> The all_reduce operation.
	 */
	virtual std::shared_ptr<host_operation> create_all_reduce(
		const send_receive_buffer &buffer,
		reduction_operation reduction
	) = 0;

	/**
	 * @brief Create a gather operation.
	 * 
	 * @param send_buffer Buffer to be sent in the gather operation.
	 * @param recv_buffer Buffer where the gathered data is written.
	 * @param root_rank Rank of the root.
	 * @return std::shared_ptr<host_operation> The gather operation.
	 */
	virtual std::shared_ptr<host_operation> create_gather(
		const send_buffer &send_buffer,
		const receive_buffer &recv_buffer,
		int root_rank
	) = 0;

	/**
	 * @brief Create a all_gather operation.
	 * 
	 * @param send_buffer Buffer to be sent in the gather operation.
	 * @param recv_buffer Buffer where the gathered data is written.
	 * @return std::shared_ptr<host_operation> The all_gather operation.
	 */
	virtual std::shared_ptr<host_operation> create_all_gather(
		const send_buffer &send_buffer,
		const receive_buffer &recv_buffer
	) = 0;

	/**
	 * @brief Create a scatter operation.
	 * 
	 * @param send_buffer Buffer to be sent in the scatter operation.
	 * @param recv_buffer Buffer where the scattered data is written.
	 * @param root_rank Rank of the root.
	 * @return std::shared_ptr<host_operation> The scatter operation.
	 */
	virtual std::shared_ptr<host_operation> create_scatter(
		const send_buffer &send_buffer,
		const receive_buffer &recv_buffer,
		int root_rank
	) = 0;

};

} // namespace communication
} // namespace xmipp4
