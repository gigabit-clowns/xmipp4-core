// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/communication/host_operation.hpp>

#include <xmipp4/core/communication/host_send_receive_regions.hpp>

namespace xmipp4 
{
namespace communication
{

/**
 * @brief Implementation of a host_operation that does nothing.
 * 
 */
class dummy_host_no_operation final
	: public host_operation
{
public:
	dummy_host_no_operation() = default;
	~dummy_host_no_operation() override = default;

	void execute() override;

};

} // namespace communication
} // namespace xmipp4
