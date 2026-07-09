// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <cpu/dispatch/cpu_program_builder.hpp>

namespace xmipp4
{
namespace dispatch
{

class cpu_copy_program_builder final
	: public cpu_program_builder
{
public:
	cpu_copy_program_builder() noexcept = default;
	~cpu_copy_program_builder() override = default;

	operation_id get_operation_id() const noexcept override;

	std::shared_ptr<hardware::program> build(
		const operation &operation,
		span<const ndarray::array_signature> output_signatures,
		span<const ndarray::array_signature> input_signatures,
		hardware::command_queue &queue,
		program_cache *cache
	) const override;
};

} // namespace dispatch
} // namespace xmipp4
