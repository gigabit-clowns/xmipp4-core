// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/multidimensional/operation_program_builder.hpp>

#include <xmipp4/core/ndarray/array_signature.hpp>
#include <xmipp4/core/hardware/program.hpp>
#include <xmipp4/core/hardware/command_queue.hpp>

#include <trompeloeil.hpp>

namespace xmipp4
{
namespace multidimensional
{

class mock_operation_program_builder
	: public operation_program_builder
{
public:
	MAKE_CONST_MOCK0(
		get_operation_id,
		operations::operation_id (),
		noexcept override
	);

	MAKE_CONST_MOCK4(
		get_suitability,
		backend_priority (
			const operations::operation &operation,
			span<const ndarray::array_signature> output_signatures,
			span<const ndarray::array_signature> input_signatures,
			hardware::command_queue &queue
		),
		override
	);

	MAKE_CONST_MOCK5(
		build,
		std::shared_ptr<hardware::program> (
			const operations::operation &operation,
			span<const ndarray::array_signature> output_signatures,
			span<const ndarray::array_signature> input_signatures,
			hardware::command_queue &queue,
			operation_program_cache *cache
		),
		override
	);
};

} // namespace multidimensional
} // namespace xmipp4
