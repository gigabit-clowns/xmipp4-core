// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "collective_communicator.hpp"
#include "reduction_operation.hpp"
#include "../numerical_type.hpp"
#include "../platform/dynamic_shared_object.h"

#include <memory>

namespace xmipp4 
{
namespace communication
{

class host_operation;
class host_send_region;
class host_receive_region;
class host_duplex_region;

/**
 * @brief Abstract class to represent interprocess and inter-node 
 * communications.
 * 
 */
class XMIPP4_CORE_API host_communicator
	: public collective_communicator<
		host_communicator,
		host_operation,
		host_send_region,
		host_receive_region,
		host_duplex_region
	>
{
public:
	host_communicator() = default;
	host_communicator(const host_communicator &other) = delete;
	host_communicator(host_communicator &&other) = delete;
	~host_communicator() override = default;

	host_communicator& operator=(const host_communicator &other) = delete;
	host_communicator& operator=(host_communicator &&other) = delete;

	/**
	 * @brief Create a barrier operation.
	 * 
	 * @return std::shared_ptr<host_operation> The barrier operation.
	 */
	virtual std::shared_ptr<host_operation> create_barrier() = 0;

};

} // namespace communication
} // namespace xmipp4
