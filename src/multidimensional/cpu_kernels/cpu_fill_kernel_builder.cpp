// SPDX-License-Identifier: GPL-3.0-only

#include "cpu_fill_kernel_builder.hpp"

#include <xmipp4/core/multidimensional/operations/fill_operation.hpp>
#include <xmipp4/core/multidimensional/array_access_layout_builder.hpp>
#include <xmipp4/core/multidimensional/array_descriptor.hpp>
#include <xmipp4/core/hardware/cpu/cpu_device.hpp>

#include "cpu_fill_kernel.hpp"

namespace xmipp4 
{
namespace multidimensional
{


operation_id 
cpu_fill_kernel_builder::get_operation_id() const noexcept
{
	return operation_id::of<fill_operation>();
}

backend_priority cpu_fill_kernel_builder::get_suitability(
	const operation&,
	span<const array_descriptor>,
	hardware::device &device
) const
{
	if (dynamic_cast<const hardware::cpu_device*>(&device) != nullptr)
	{
		return backend_priority::normal;
	}

	return backend_priority::unsupported;
}

std::shared_ptr<kernel> cpu_fill_kernel_builder::build(
	const operation&,
	span<const array_descriptor> descriptors,
	hardware::device&
) const
{
	array_access_layout_builder layout_builder;
	for (const auto &descriptor : descriptors)
	{
		layout_builder.add_operand(descriptor.get_layout());
	}

	return std::make_shared<cpu_fill_kernel>(
		layout_builder.build(),
		descriptors[0].get_data_type(),
		descriptors[1].get_data_type()
	);
}

} // namespace multidimensional
} // namespace xmipp4
