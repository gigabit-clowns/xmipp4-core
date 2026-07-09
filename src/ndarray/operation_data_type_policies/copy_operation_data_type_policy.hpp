// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/dispatch/operation_data_type_policy.hpp>
#include <xmipp4/core/platform/dynamic_shared_object.h>

namespace xmipp4
{
namespace dispatch
{

/**
 * @brief Data type policy allowing converting an input type into an
 * output type.
 */
class XMIPP4_CORE_API copy_operation_data_type_policy final
	: public operation_data_type_policy
{
public:
	void deduce(
		span<numerical_type> canonical_output_types,
		span<const numerical_type> input_types
	) const override;

	void accept(
		span<const numerical_type> user_output_types,
		span<const numerical_type> canonical_output_types,
		span<const numerical_type> input_types
	) const override;

	static const copy_operation_data_type_policy& get() noexcept;
};

} // namespace dispatch
} // namespace xmipp4
