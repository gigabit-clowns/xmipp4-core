// SPDX-License-Identifier: GPL-3.0-only

#include "cpu_copy_kernel_builder.hpp"

#include <xmipp4/core/multidimensional/operations/copy_operation.hpp>
#include <xmipp4/core/multidimensional/array_access_layout_builder.hpp>
#include <xmipp4/core/multidimensional/array_descriptor.hpp>
#include <xmipp4/core/hardware/cpu/cpu_device.hpp>

#include "cpu_copy_kernel.hpp"

namespace xmipp4 
{
namespace multidimensional
{


operation_id 
cpu_copy_kernel_builder::get_operation_id() const noexcept
{
	return operation_id::of<copy_operation>();
}

backend_priority cpu_copy_kernel_builder::get_suitability(
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

std::shared_ptr<kernel> cpu_copy_kernel_builder::build(
	const operation&,
	span<const array_descriptor> descriptors,
	hardware::device&
) const
{
	if (descriptors.size() != 2)
	{
		throw std::invalid_argument(
			"cpu_copy_kernel_builder::build: Expected exactly 2 "
			"array descriptors."
		);
	}

	array_access_layout_builder layout_builder;
	for (const auto &descriptor : descriptors)
	{
		layout_builder.add_operand(descriptor.get_layout());
	}

	auto access_layout = layout_builder.build();

	return nullptr; // TODO
}

} // namespace multidimensional
} // namespace xmipp4
