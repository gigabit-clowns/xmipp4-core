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
 * @brief Copy elements from an input array to an output array.
 */
class copy_operation
	: public operation
{
public:
	XMIPP4_CORE_API copy_operation() noexcept;
	XMIPP4_CORE_API ~copy_operation() override;
	
	XMIPP4_CORE_API std::string get_name() const override;

	XMIPP4_CORE_API
	void sanitize_operands(
		span<array_descriptor> output_descriptors,
		span<array_descriptor> input_descriptors
	) const override;
};

} // namespace multidimensional
} // namespace xmipp4
