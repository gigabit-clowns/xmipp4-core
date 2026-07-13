// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/backends/cpu/program_builder.hpp>

namespace xmipp4
{
namespace cpu
{

class add_program_builder final
	: public program_builder
{
public:
	add_program_builder() noexcept = default;
	~add_program_builder() override = default;

	operation_id get_operation_id() const noexcept override;

	std::shared_ptr<xmipp4::program> build(
		const operation &operation,
		span<const operand_signature> output_signatures,
		span<const operand_signature> input_signatures,
		xmipp4::command_queue &queue,
		program_cache *cache
	) const override;
};

} // namespace cpu
} // namespace xmipp4
