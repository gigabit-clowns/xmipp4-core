// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/multidimensional/kernel.hpp>

#include <xmipp4/core/hardware/buffer.hpp>
#include <xmipp4/core/hardware/device_queue.hpp>

#include <trompeloeil.hpp>

namespace xmipp4
{
namespace multidimensional
{

class mock_kernel
	: public kernel
{
public:
	MAKE_CONST_MOCK3(
		execute, 
		void(
			span<const std::shared_ptr<hardware::buffer>> read_write_operands,
			span<const std::shared_ptr<const hardware::buffer>> read_only_operands,
			hardware::device_queue *queue
		),
		override
	);
};

} // namespace multidimensional
} // namespace xmipp4
