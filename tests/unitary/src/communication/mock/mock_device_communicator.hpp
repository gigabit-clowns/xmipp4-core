// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/communication/device_communicator.hpp>

#include <trompeloeil.hpp>

namespace xmipp4
{
namespace communication
{

class mock_device_communicator final
	: public device_communicator
{
public:
	MAKE_CONST_MOCK0(get_size, std::size_t(), override);
	MAKE_CONST_MOCK0(get_rank, std::size_t(), override);
	MAKE_CONST_MOCK0(get_device, hardware::device&(), noexcept override);
	MAKE_CONST_MOCK2(
		split, 
		std::shared_ptr<device_communicator>(int, int), 
		override
	);
	MAKE_MOCK2(
		create_send,
		std::shared_ptr<device_operation>(
			const device_send_region &region, 
			int destination_rank
		),
		override
	);
	MAKE_MOCK2(
		create_receive,
		std::shared_ptr<device_operation>(
			const device_receive_region &region, 
			int source_rank
		),
		override
	);
	MAKE_MOCK2(
		create_broadcast,
		std::shared_ptr<device_operation>(
			const device_send_receive_regions &regions, 
			int root_rank
		),
		override
	);
	MAKE_MOCK3(
		create_reduce,
		std::shared_ptr<device_operation>(
			const device_send_receive_regions &regions, 
			reduction_operation reduction,
			int root_rank
		),
		override
	);
	MAKE_MOCK2(
		create_all_reduce,
		std::shared_ptr<device_operation>(
			const device_send_receive_regions &regions, 
			reduction_operation reduction
		),
		override
	);
	MAKE_MOCK3(
		create_gather,
		std::shared_ptr<device_operation>(
			const device_send_region &send_region,
			const device_receive_region &recv_region,
			int root_rank
		),
		override
	);
	MAKE_MOCK2(
		create_all_gather,
		std::shared_ptr<device_operation>(
			const device_send_region &send_region,
			const device_receive_region &recv_region
		),
		override
	);
	MAKE_MOCK3(
		create_scatter,
		std::shared_ptr<device_operation>(
			const device_send_region &send_region,
			const device_receive_region &recv_region,
			int root_rank
		),
		override
	);

};

} // namespace communication
} // namespace xmipp4
