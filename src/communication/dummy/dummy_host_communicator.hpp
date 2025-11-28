// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/communication/host_communicator.hpp>

#include <xmipp4/core/platform/attributes.hpp>


namespace xmipp4 
{
namespace communication
{

/**
 * @brief Dummy implementation of host_communicator that represents a rank
 * that is only able to communicate with itself.
 * 
 */
class dummy_host_communicator final
	: public host_communicator
{
public:
	std::size_t get_size() const override;

	std::size_t get_rank() const override;

	std::shared_ptr<host_communicator> split(
		int colour, 
		int rank_priority
	) const override;

	XMIPP4_NORETURN
	std::shared_ptr<host_operation> create_send(
		const host_send_region &buffer,
		int destination_rank,
		int tag
	) override;

	XMIPP4_NORETURN
	std::shared_ptr<host_operation> create_receive(
		const host_receive_region &region,
		int source_rank,
		int tag
	) override;

	std::shared_ptr<host_operation> create_broadcast(
		const host_duplex_region &region,
		int root_rank
	) override;

	std::shared_ptr<host_operation> create_reduce(
		const host_duplex_region &region,
		reduction_operation reduction,
		int root_rank
	) override;

	std::shared_ptr<host_operation> create_all_reduce(
		const host_duplex_region &region,
		reduction_operation reduction
	) override;

	std::shared_ptr<host_operation> create_gather(
		const host_send_region &send_region,
		const host_receive_region &recv_region,
		int root_rank
	) override;

	std::shared_ptr<host_operation> create_all_gather(
		const host_send_region &send_region,
		const host_receive_region &recv_region
	) override;

	std::shared_ptr<host_operation> create_scatter(
		const host_send_region &send_region,
		const host_receive_region &recv_region,
		int root_rank
	) override;

	std::shared_ptr<host_operation> create_barrier() override;

private:
	static void validate_root_rank(int root_rank);
	static std::shared_ptr<host_operation> create_operation(
		const host_duplex_region &region,
		int root_rank
	);
	static std::shared_ptr<host_operation> create_operation(
		const host_duplex_region &region
	);
	static std::shared_ptr<host_operation> create_operation(
		const host_send_region &send_region,
		const host_receive_region &recv_region,
		int root_rank
	);
	static std::shared_ptr<host_operation> create_operation(
		const host_send_region &send_region,
		const host_receive_region &recv_region
	);

};

} // namespace communication
} // namespace xmipp4
