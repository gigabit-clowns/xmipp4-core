// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/communication/host_communicator.hpp>

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

	void barrier() override;
};

} // namespace communication
} // namespace xmipp4
