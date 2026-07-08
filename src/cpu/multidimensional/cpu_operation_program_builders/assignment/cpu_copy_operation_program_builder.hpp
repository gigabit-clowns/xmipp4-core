// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <cpu/multidimensional/cpu_operation_program_builder.hpp>

namespace xmipp4 
{
namespace multidimensional
{

class cpu_copy_operation_program_builder final
	: public cpu_operation_program_builder
{
public:
	cpu_copy_operation_program_builder() noexcept = default;
	~cpu_copy_operation_program_builder() override = default;

	operation_id get_operation_id() const noexcept override;

	std::shared_ptr<hardware::program> build(
		const operation &operation,
		span<const ndarray::array_signature> output_signatures,
		span<const ndarray::array_signature> input_signatures,
		hardware::command_queue &queue,
		operation_program_cache *cache
	) const override;
};

} // namespace multidimensional
} // namespace xmipp4
