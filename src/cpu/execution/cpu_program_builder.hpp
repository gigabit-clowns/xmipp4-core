// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/execution/program_builder.hpp>

namespace xmipp4 
{
namespace execution
{

class cpu_program_builder
	: public program_builder
{
public:
	backend_priority get_suitability(
		const operations::operation &operation,
		span<const ndarray::array_signature> output_signatures,
		span<const ndarray::array_signature> input_signatures,
		hardware::command_queue &queue
	) const override;
};

} // namespace execution
} // namespace xmipp4

