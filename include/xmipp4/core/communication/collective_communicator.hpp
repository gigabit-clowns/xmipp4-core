// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "communicator.hpp"
#include "reduction_operation.hpp"
#include "../platform/dynamic_shared_object.h"

#include <memory>

namespace xmipp4 
{
namespace communication
{

/**
 * @brief Interface declaration for collective operations.
 * 
 * This class contains defines collective communication patterns that define the 
 * common interface of host_communicator and device_communicator.
 * 
 */
template <
	typename Comm,
	typename OpType, 
	typename SendRegion, 
	typename RecvRegion,
	typename DuplexRegion
>
class XMIPP4_CORE_API collective_communicator
	: public communicator
{
public:
	using communicator_type = Comm;
	using operation_type = OpType;
	using send_region_type = SendRegion;
	using receive_region_type = RecvRegion;
	using duplex_region_type = DuplexRegion;

	collective_communicator() = default;
	collective_communicator(const collective_communicator &other) = delete;
	collective_communicator(collective_communicator &&other) = delete;
	~collective_communicator() override = default;

	collective_communicator& 
	operator=(const collective_communicator &other) = delete;
	collective_communicator& 
	operator=(collective_communicator &&other) = delete;

	/**
	 * @brief Split the current communicator group.
	 * 
	 * @param colour The group where the current rank will be assigned to.
	 * @param rank_priority Hint to assign the rank in the new communicator.
	 * @return std::shared_ptr<communicator_type> The communicator where
	 * the current rank has been assigned to.
	 * 
	 */
	virtual std::shared_ptr<communicator_type> split(
		int colour, 
		int rank_priority
	) const = 0;

	/**
	 * @brief Create a send operation.
	 * 
	 * The send operation can be used to transmit the contents of the provided
	 * memory region to another peer awaiting reception. Thus, the 
	 * destination_rank must be calling execute on an operation returned by
	 * create_receive with the source rank equal to this.
	 * 
	 * @param region Buffer to be sent. Its contents must be valid during the 
	 * lifetime of the operation.
	 * @param destination_rank Rank of the peer to which data is sent.
	 * @param tag An label to identify the messages sent by de resulting 
	 * operation.
	 * @return std::shared_ptr<operation_type> The send operation.
	 *
	 * @see create_receive
	 */
	virtual std::shared_ptr<operation_type> create_send(
		const send_region_type &region,
		int destination_rank,
		int tag
	) = 0;

	/**
	 * @brief Create a receive operation.
	 * 
	 * The receive operation can be used to receive data into the provided
	 * memory region from another peer sending the data. Thus, the source_rank 
	 * must be calling execute on an operation returned by create_send with the
	 * source rank equal to this.
	 * 
	 * @param region Buffers where the received contents are written. Its 
	 * contents must be valid during the lifetime of the operation.
	 * @param source_rank Rank of the peer sending the data.
	 * @param tag Label to filter messages.
	 * @return std::shared_ptr<operation_type> The receive operation.
	 */
	virtual std::shared_ptr<operation_type> create_receive(
		const receive_region_type &region,
		int source_rank,
		int tag
	) = 0;

	/**
	 * @brief Create a broadcast operation.
	 * 
	 * The broadcast operation transmits the data contained in the root
	 * peer to the rest of the peers. 
	 * 
	 * @param region Buffers where the broadcast happens. Send region on all 
	 * peers except the root is unused. Root rank can alias the same region for 
	 * sending and receiving data.
	 * @param root_rank Rank of the root.
	 * @return std::shared_ptr<operation_type> The broadcast operation.
	 */
	virtual std::shared_ptr<operation_type> create_broadcast(
		const duplex_region_type &region,
		int root_rank
	) = 0;

	/**
	 * @brief Create a reduce operation.
	 * 
	 * The reduction operation reduces the send region element-wise across peers 
	 * according to the requested reduction operation. Root rank obtains the 
	 * reduced version of the region.
	 * 
	 * @param region Buffers where the the reduction happens. Ignored for all 
	 * peers except the root. The reception region can alias the send buffer.
	 * @param reduction Type of the reduction operation.
	 * @param root_rank Rank of the root.
	 * @return std::shared_ptr<operation_type> The reduce operation.
	 */
	virtual std::shared_ptr<operation_type> create_reduce(
		const duplex_region_type &region,
		reduction_operation reduction,
		int root_rank
	) = 0;

	/**
	 * @brief Create an all_reduce operation.
	 * 
	 * The reduction operation combines the send region element-wise across 
	 * peers according to the requested reduction operation. All ranks obtain 
	 * the reduced version of the region. Receive buffer can alias the send 
	 * buffer.
	 * 
	 * @param region Buffers where the the reduction happens. The reception
	 * region can alias the send buffer.
	 * @param region Buffers where the the reduction happens.
	 * @param reduction Type of the reduction operation.
	 * @return std::shared_ptr<operation_type> The all_reduce operation.
	 */
	virtual std::shared_ptr<operation_type> create_all_reduce(
		const duplex_region_type &region,
		reduction_operation reduction
	) = 0;

	/**
	 * @brief Create a gather operation.
	 * 
	 * The gather operation concatenates the contents of all peer's regions
	 * into the receive region of the root.
	 * 
	 * @param send_region Buffer to be sent in the gather operation.
	 * @param recv_region Buffer where the gathered data is written. Its count 
	 * must be equal to the send_region's count times the size of the 
	 * communicator.
	 * @param root_rank Rank of the root.
	 * @return std::shared_ptr<operation_type> The gather operation.
	 */
	virtual std::shared_ptr<operation_type> create_gather(
		const send_region_type &send_region,
		const receive_region_type &recv_region,
		int root_rank
	) = 0;

	/**
	 * @brief Create a all_gather operation.
	 * 
	 * The gather operation concatenates the contents of all peer's regions
	 * into the receive region of all peers.
	 * 
	 * @param send_region Buffer to be sent in the gather operation.
	 * @param recv_region Buffer where the gathered data is written. Its count 
	 * must be equal to the send_region's count times the size of the 
	 * communicator.
	 * @return std::shared_ptr<operation_type> The all_gather operation.
	 */
	virtual std::shared_ptr<operation_type> create_all_gather(
		const send_region_type &send_region,
		const receive_region_type &recv_region
	) = 0;

	/**
	 * @brief Create a scatter operation.
	 * 
	 * The scatter operation partitions the contents of the send region and 
	 * distributes them across all peers.
	 * 
	 * @param send_region Buffer to be sent in the scatter operation. Ignored
	 * for all peers except the root. Its count must be equal to the 
	 * recv_region's count times the size of the communicator.
	 * @param recv_region Buffer where the scattered data is written.
	 * @param root_rank Rank of the root.
	 * @return std::shared_ptr<operation_type> The scatter operation.
	 */
	virtual std::shared_ptr<operation_type> create_scatter(
		const send_region_type &send_region,
		const receive_region_type &recv_region,
		int root_rank
	) = 0;
};

} // namespace communication
} // namespace xmipp4
