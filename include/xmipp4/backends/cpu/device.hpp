// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/hardware/device.hpp>

namespace xmipp4 
{
namespace cpu
{

/**
 * @brief Implementation of the @ref device interface to represent the CPU.
 */
class XMIPP4_CORE_API device final
	: public xmipp4::device
{
public:
	device() noexcept;
	~device() override;

	const memory_resource& 
	get_memory_resource(memory_resource_affinity affinity) const override;

	std::shared_ptr<xmipp4::command_queue> create_command_queue() const override;

	std::shared_ptr<xmipp4::event>
	create_event(event_usage_flags usage) const override;
};

} // namespace cpu
} // namespace xmipp4
