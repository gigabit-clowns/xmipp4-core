// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/hardware/memory_transfer.hpp>

namespace xmipp4 
{
namespace hardware
{

class host_memory_transfer final
	: public memory_transfer
{
public:
	host_memory_transfer() = default;
	~host_memory_transfer() override = default;

	void copy(
		const buffer &source, 
		buffer &destination,
		span<const copy_region> regions, 
		device_queue *queue
	) const override;
};

} // namespace hardware
} // namespace xmipp4
