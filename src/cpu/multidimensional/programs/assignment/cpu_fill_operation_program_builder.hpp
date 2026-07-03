// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/multidimensional/operation_program_builder.hpp>

namespace xmipp4 
{
namespace multidimensional
{

class cpu_fill_operation_program_builder final
	: public operation_program_builder
{
public:
	cpu_fill_operation_program_builder() noexcept = default;
	~cpu_fill_operation_program_builder() override = default;

	operation_id get_operation_id() const noexcept override;

	backend_priority get_suitability(
		const operation &operation,
		span<const array_signature> output_signatures,
		span<const array_signature> input_signatures,
		hardware::command_queue &queue
	) const override;

	std::shared_ptr<hardware::program> build(
		const operation &operation,
		span<const array_signature> output_signatures,
		span<const array_signature> input_signatures,
		hardware::command_queue &queue,
		operation_program_cache *cache
	) const override;
};

} // namespace multidimensional
} // namespace xmipp4
