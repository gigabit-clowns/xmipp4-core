// SPDX-License-Identifier: GPL-3.0-only

#include "dummy_host_communicator.hpp"

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

void dummy_host_communicator::barrier()
{
	// No-op
}

} // namespace communication
} // namespace xmipp4
