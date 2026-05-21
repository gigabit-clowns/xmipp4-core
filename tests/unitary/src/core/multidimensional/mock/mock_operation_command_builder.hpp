// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/multidimensional/operation_command_builder.hpp>

#include <xmipp4/core/multidimensional/array_signature.hpp>

#include <trompeloeil.hpp>

namespace xmipp4
{
namespace multidimensional
{

class mock_operation_command_builder
	: public operation_command_builder
{
public:
	MAKE_CONST_MOCK0(
		get_operation_id, 
		operation_id (), 
		noexcept override
	);


	MAKE_CONST_MOCK3(
		get_suitability, 
		backend_priority (
			const operation &operation,
			span<const array_signature> output_signatures,
			span<const array_signature> input_signatures
		), 
		override
	);

	MAKE_CONST_MOCK3(
		build, 
		std::shared_ptr<kernel> (
			const operation &operation,
			span<const array_signature> output_signatures,
			span<const array_signature> input_signatures
		), 
		override
	);
};

} // namespace multidimensional
} // namespace xmipp4
