// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/multidimensional/operation_dispatcher.hpp>

#include <xmipp4/core/multidimensional/operation.hpp>
#include <xmipp4/core/multidimensional/array.hpp>
#include <xmipp4/core/multidimensional/array_view.hpp>
#include <xmipp4/core/hardware/device_context.hpp>
#include <xmipp4/core/hardware/command_queue.hpp>

#include <trompeloeil.hpp>

namespace xmipp4
{
namespace multidimensional
{

class mock_operation_dispatcher final
	: public operation_dispatcher
{
public:
	MAKE_MOCK5(
		dispatch,
		void (
			const operation &operation,
			span<array> output_operands,
			span<const array_view> input_operands,
			const hardware::device_context &device_context,
			hardware::command_queue &queue
		),
		override
	);
};

} // namespace multidimensional
} // namespace xmipp4
