// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/dispatch/dispatcher.hpp>

#include <xmipp4/core/operations/operation.hpp>
#include <xmipp4/core/ndarray/array.hpp>
#include <xmipp4/core/ndarray/array_view.hpp>
#include <xmipp4/core/hardware/device_context.hpp>
#include <xmipp4/core/hardware/command_queue.hpp>

#include <trompeloeil.hpp>

namespace xmipp4
{
namespace dispatch
{

class mock_dispatcher final
	: public dispatcher
{
public:
	MAKE_MOCK4(
		dispatch,
		void (
			const operations::operation &operation,
			span<ndarray::array> output_operands,
			span<const ndarray::array_view> input_operands,
			const hardware::device_context &device_context
		),
		override
	);
};

} // namespace dispatch
} // namespace xmipp4
