// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "../platform/dynamic_shared_object.h"

#include <memory>
#include <cstddef>

namespace xmipp4 
{
namespace communication
{

/**
 * @brief Abstract class to represent a communicator.
 * 
 * This class provides the common interface shared by host_communicator
 * and device_communicator. 
 * 
 */
class XMIPP4_CORE_API communicator
{
public:
	communicator() = default;
	communicator(const communicator &other) = delete;
	communicator(communicator &&other) = delete;
	virtual ~communicator() = default;

	communicator& operator=(const communicator &other) = delete;
	communicator& operator=(communicator &&other) = delete;

	/**
	 * @brief Get the amount of peers in the communicator.
	 * 
	 * @return std::size_t The number of peers.
	 * 
	 */
	virtual std::size_t get_size() const = 0;

	/**
	 * @brief Get the rank of this communicator peer.
	 * 
	 * The rank refers to the "id" or "index" of this project unique to
	 * this peer.
	 * 
	 * @return std::size_t The rank.
	 * 
	 */
	virtual std::size_t get_rank() const = 0;

};

} // namespace communication
} // namespace xmipp4
