// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/multidimensional/operations/arithmetic_operation.hpp>
#include <xmipp4/core/platform/dynamic_shared_object.h>
#include <xmipp4/core/multidimensional/data_type_policies/abs_data_type_policy.hpp>

namespace xmipp4
{
namespace multidimensional
{

/**
 * @brief Compute `abs(x)` for all elements.
 */
class XMIPP4_CORE_API abs_operation final
	: public arithmetic_operation
{
public:
	std::string get_name() const override;
	std::size_t get_output_count() const noexcept override;
	std::size_t get_input_count() const noexcept override;
	const data_type_policy& get_data_type_policy() const noexcept override;
};

} // namespace multidimensional
} // namespace xmipp4
