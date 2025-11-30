// SPDX-License-Identifier: GPL-3.0-only

#include "dummy_host_communicator.hpp"

#include <xmipp4/core/communication/host_send_region.hpp>
#include <xmipp4/core/communication/host_duplex_region.hpp>
#include <xmipp4/core/communication/host_receive_region.hpp>
#include <xmipp4/core/exceptions/invalid_operation_error.hpp>

#include "dummy_host_copy_operation.hpp"
#include "dummy_host_no_operation.hpp"

namespace xmipp4 
{
namespace communication
{

std::size_t dummy_host_communicator::get_size() const
{
	return 1;
}

std::size_t dummy_host_communicator::get_rank() const
{
	return 0;
}

std::shared_ptr<host_communicator> 
dummy_host_communicator::split(int, int) const
{
	return std::make_shared<dummy_host_communicator>();
}

std::shared_ptr<host_operation> dummy_host_communicator::create_send(
	const host_send_region&, 
	std::size_t destination_rank,
	int
)
{
	if (destination_rank == 0)
	{
		throw std::invalid_argument(
			"Can not send data on the same rank that is receiving it"
		);
	}

	throw std::out_of_range("destination_rank is out of bounds");
}

std::shared_ptr<host_operation> dummy_host_communicator::create_receive(
	const host_receive_region&, 
	std::size_t source_rank,
	int
)
{
	if (source_rank == 0)
	{
		throw std::invalid_argument(
			"Can not receive data on the same rank that is sending it"
		);
	}

	throw std::out_of_range("source_rank is out of bounds");
}

std::shared_ptr<host_operation> dummy_host_communicator::create_broadcast(
	const host_duplex_region &regions,
	std::size_t root_rank
) 
{
	return create_operation(regions, root_rank);
}

std::shared_ptr<host_operation> dummy_host_communicator::create_reduce(
	const host_duplex_region &regions,
	reduction_operation,
	std::size_t root_rank
)
{
	return create_operation(regions, root_rank);
}

std::shared_ptr<host_operation> dummy_host_communicator::create_all_reduce(
	const host_duplex_region &regions,
	reduction_operation
)
{
	return create_operation(regions);
}

std::shared_ptr<host_operation> dummy_host_communicator::create_gather(
	const host_send_region &send_region,
	const host_receive_region &recv_region,
	std::size_t root_rank
)
{
	return create_operation(send_region, recv_region, root_rank);
}

std::shared_ptr<host_operation> dummy_host_communicator::create_all_gather(
	const host_send_region &send_region,
	const host_receive_region &recv_region
)
{
	return create_operation(send_region, recv_region);
}

std::shared_ptr<host_operation> dummy_host_communicator::create_scatter(
	const host_send_region &send_region,
	const host_receive_region &recv_region,
	std::size_t root_rank
)
{
	return create_operation(send_region, recv_region, root_rank);
}

std::shared_ptr<host_operation> dummy_host_communicator::create_barrier()
{
	return std::make_shared<dummy_host_no_operation>();
}

void dummy_host_communicator::validate_root_rank(std::size_t root_rank)
{
	if (root_rank != 0)
	{
		throw std::out_of_range(
			"The root_rank in a dummy_communicator can only be 0"
		);
	}
}

std::shared_ptr<host_operation> dummy_host_communicator::create_operation(
	const host_duplex_region &regions,
	std::size_t root_rank
)
{
	validate_root_rank(root_rank);
	return create_operation(regions);
}

std::shared_ptr<host_operation> dummy_host_communicator::create_operation(
	const host_duplex_region &regions
)
{
	if (regions.get_send_data() == regions.get_receive_data())
	{
		return std::make_shared<dummy_host_no_operation>();
	}
	else
	{
		return std::make_shared<dummy_host_copy_operation>(
			regions.get_send_data(),
			regions.get_receive_data(),
			xmipp4::get_size(regions.get_data_type()) * regions.get_count()
		);
	}
}

std::shared_ptr<host_operation> dummy_host_communicator::create_operation(
	const host_send_region &send_region,
	const host_receive_region &recv_region,
	std::size_t root_rank
)
{
	validate_root_rank(root_rank);
	return create_operation(send_region, recv_region);
}

std::shared_ptr<host_operation> dummy_host_communicator::create_operation(
	const host_send_region &send_region,
	const host_receive_region &recv_region
)
{
	if (send_region.get_data_type() != recv_region.get_data_type())
	{
		throw std::invalid_argument(
			"Send and receive regions must have the same data type"
		);
	}

	if (send_region.get_count() != recv_region.get_count())
	{
		throw std::invalid_argument(
			"Send and receive regions must have the same element count"
		);
	}

	return std::make_shared<dummy_host_copy_operation>(
		send_region.get_data(),
		recv_region.get_data(),
		xmipp4::get_size(send_region.get_data_type()) * send_region.get_count()
	);
}

} // namespace communication
} // namespace xmipp4
