// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/communication/host_communicator.hpp>

#include <xmipp4/core/communication/host_send_region.hpp>
#include <xmipp4/core/communication/host_send_receive_regions.hpp>
#include <xmipp4/core/communication/host_receive_region.hpp>

#include <trompeloeil.hpp>

namespace xmipp4
{
namespace communication
{

class mock_host_communicator final
	: public host_communicator
{
public:
	MAKE_CONST_MOCK0(get_size, std::size_t(), override);
	MAKE_CONST_MOCK0(get_rank, std::size_t(), override);
	MAKE_CONST_MOCK2(
		split, 
		std::shared_ptr<host_communicator>(int, int), 
		override
	);
	MAKE_MOCK3(
		create_send,
		std::shared_ptr<host_operation>(
			const host_send_region &region, 
			int destination_rank,
			int tag
		),
		override
	);
	MAKE_MOCK3(
		create_receive,
		std::shared_ptr<host_operation>(
			const host_receive_region &region, 
			int source_rank,
			int tag
		),
		override
	);
	MAKE_MOCK2(
		create_broadcast,
		std::shared_ptr<host_operation>(
			const host_send_receive_regions &regions, 
			int root_rank
		),
		override
	);
	MAKE_MOCK3(
		create_reduce,
		std::shared_ptr<host_operation>(
			const host_send_receive_regions &regions, 
			reduction_operation reduction,
			int root_rank
		),
		override
	);
	MAKE_MOCK2(
		create_all_reduce,
		std::shared_ptr<host_operation>(
			const host_send_receive_regions &regions, 
			reduction_operation reduction
		),
		override
	);
	MAKE_MOCK3(
		create_gather,
		std::shared_ptr<host_operation>(
			const host_send_region &send_region,
			const host_receive_region &recv_region,
			int root_rank
		),
		override
	);
	MAKE_MOCK2(
		create_all_gather,
		std::shared_ptr<host_operation>(
			const host_send_region &send_region,
			const host_receive_region &recv_region
		),
		override
	);
	MAKE_MOCK3(
		create_scatter,
		std::shared_ptr<host_operation>(
			const host_send_region &send_region,
			const host_receive_region &recv_region,
			int root_rank
		),
		override
	);
	MAKE_MOCK0(
		create_barrier,
		std::shared_ptr<host_operation>(),
		override
	);

};

} // namespace communication
} // namespace xmipp4
