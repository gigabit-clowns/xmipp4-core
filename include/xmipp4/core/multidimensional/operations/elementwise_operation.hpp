// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "../operation.hpp"
#include "../../platform/dynamic_shared_object.h"

#include <string>

namespace xmipp4 
{
namespace multidimensional
{

/**
 * @brief Abstract representation for operations where each output element only 
 * depends on the inputs at the same position.
 */
class elementwise_operation
	: public operation
{
public:
	XMIPP4_CORE_API elementwise_operation() noexcept;
	XMIPP4_CORE_API ~elementwise_operation() override;

	void sanitize_operands(
		span<array_descriptor> output_descriptors,
		span<array_descriptor> input_descriptors
	) const override;
};

} // namespace multidimensional
} // namespace xmipp4
