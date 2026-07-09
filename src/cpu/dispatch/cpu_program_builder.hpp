// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/dispatch/program_builder.hpp>

namespace xmipp4
{
namespace dispatch
{

class cpu_program_builder
	: public program_builder
{
public:
	backend_priority get_suitability(
		const operation &operation,
		span<const ndarray::array_signature> output_signatures,
		span<const ndarray::array_signature> input_signatures,
		hardware::command_queue &queue
	) const override;
};

} // namespace dispatch
} // namespace xmipp4

