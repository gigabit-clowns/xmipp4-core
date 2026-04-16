// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/multidimensional/operation.hpp>

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
	MAKE_CONST_MOCK0(
		get_shape_policy, 
		const shape_policy&(), 
		noexcept override
	);
	MAKE_CONST_MOCK0(
		get_data_type_policy, 
		const data_type_policy&(), 
		noexcept override
	);
};

} // namespace multidimensional
} // namespace xmipp4
