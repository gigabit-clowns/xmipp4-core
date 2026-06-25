// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/multidimensional/operations/floating_elementwise_operation.hpp>
#include <xmipp4/core/platform/dynamic_shared_object.h>

namespace xmipp4
{
namespace multidimensional
{

/**
 * @brief Compute `sin(x)` for all elements.
 *
 * Input and output dtypes must be equal and they must be floating-point types.
 */
class XMIPP4_CORE_API sin_operation final
	: public floating_elementwise_operation
{
public:
	std::string get_name() const override;
	operation_arity get_arity() const noexcept override;
};

} // namespace multidimensional
} // namespace xmipp4
