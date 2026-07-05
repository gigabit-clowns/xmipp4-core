// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/multidimensional/operation_program_builder.hpp>

namespace xmipp4 
{
namespace multidimensional
{

class cpu_operation_program_builder
	: public operation_program_builder
{
public:
	backend_priority get_suitability(
		const operation &operation,
		span<const array_signature> output_signatures,
		span<const array_signature> input_signatures,
		hardware::command_queue &queue
	) const override;
};

} // namespace multidimensional
} // namespace xmipp4

