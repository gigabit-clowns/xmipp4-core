// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/multidimensional/kernel_builder.hpp>

#include <xmipp4/core/multidimensional/array_descriptor.hpp>
#include <xmipp4/core/hardware/device.hpp>

#include <trompeloeil.hpp>

namespace xmipp4
{
namespace multidimensional
{

class mock_kernel_builder
	: public kernel_builder
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
			span<const array_descriptor> descriptors,
			hardware::device &device
		), 
		override
	);

	MAKE_CONST_MOCK3(
		build, 
		std::shared_ptr<kernel> (
			const operation &operation,
			span<const array_descriptor> descriptors,
			hardware::device &device
		), 
		override
	);
};

} // namespace multidimensional
} // namespace xmipp4
