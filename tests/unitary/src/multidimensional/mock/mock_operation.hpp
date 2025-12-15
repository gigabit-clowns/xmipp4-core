// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/multidimensional/operation.hpp>

#include <xmipp4/core/multidimensional/array_descriptor.hpp>

#include <trompeloeil.hpp>

namespace xmipp4
{
namespace multidimensional
{

class mock_operation
	: public operation
{
public:
	MAKE_CONST_MOCK0(get_name, std::string(), override);
	MAKE_CONST_MOCK0(serialize_parameters, std::string(), override);
	MAKE_CONST_MOCK2(
		sanitize_operands, 
		void(span<array_descriptor>, span<array_descriptor>), 
		override
	);
};

} // namespace multidimensional
} // namespace xmipp4

