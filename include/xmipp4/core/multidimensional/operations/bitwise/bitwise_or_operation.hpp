// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/multidimensional/operations/bitwise_operation.hpp>
#include <xmipp4/core/platform/dynamic_shared_object.h>

namespace xmipp4
{
namespace multidimensional
{

/**
 * @brief Compute `x | y` for all elements.
 */
class XMIPP4_CORE_API bitwise_or_operation final
	: public bitwise_operation
{
public:
	std::string get_name() const override;
	operation_arity get_arity() const noexcept override;
};

} // namespace multidimensional
} // namespace xmipp4
