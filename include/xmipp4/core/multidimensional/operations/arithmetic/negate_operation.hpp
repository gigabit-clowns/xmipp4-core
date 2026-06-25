// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/multidimensional/operations/arithmetic_operation.hpp>
#include <xmipp4/core/platform/dynamic_shared_object.h>

namespace xmipp4
{
namespace multidimensional
{

/**
 * @brief Compute `-x` for all elements.
 */
class XMIPP4_CORE_API negate_operation final
	: public arithmetic_operation
{
public:
	std::string get_name() const override;
	operation_arity get_arity() const noexcept override;
};

} // namespace multidimensional
} // namespace xmipp4
