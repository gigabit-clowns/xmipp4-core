// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/multidimensional/operations/compare_operation.hpp>
#include <xmipp4/core/platform/dynamic_shared_object.h>

namespace xmipp4
{
namespace multidimensional
{

/**
 * @brief Computes `x <= y`.
 */
class XMIPP4_CORE_API less_equal_operation final
	: public compare_operation
{
public:
	std::string get_name() const override;
};

} // namespace multidimensional
} // namespace xmipp4
