// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/multidimensional/kernel_builder.hpp>

namespace xmipp4 
{
namespace multidimensional
{

class cpu_fill_kernel_builder final
	: public kernel_builder
{
public:
	cpu_fill_kernel_builder() noexcept = default;
	~cpu_fill_kernel_builder() override = default;

	operation_id get_operation_id() const noexcept override;

	backend_priority get_suitability(
		const operation &operation,
		span<const array_descriptor> descriptors,
		hardware::device &device
	) const override;

	std::shared_ptr<kernel> build(
		const operation &operation,
		span<const array_descriptor> descriptors,
		hardware::device &device
	) const override;
};

} // namespace multidimensional
} // namespace xmipp4
