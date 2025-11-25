// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "communicator.hpp"
#include "../platform/dynamic_shared_object.h"

#include <memory>
#include <cstddef>

namespace xmipp4 
{
namespace communication
{

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
	 * @brief Synchronize all peers.
	 * 
	 * Wait until all peers have reached this call.
	 * 
	 * @note All ranks of the communicator need to call this method.
	 * 
	 */
	virtual void barrier() = 0;

};

} // namespace communication
} // namespace xmipp4
