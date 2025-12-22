// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/multidimensional/kernel.hpp>

#include <xmipp4/core/multidimensional/array_access_layout.hpp>

namespace xmipp4 
{
namespace multidimensional
{

class cpu_copy_kernel final
	: public kernel
{
public:
	cpu_copy_kernel(
		array_access_layout access_layout,
		numerical_type output_data_type,
		numerical_type input_data_type
	) noexcept;
	~cpu_copy_kernel() override = default;

	void execute(
		span<const std::shared_ptr<hardware::buffer>> read_write_operands,
		span<const std::shared_ptr<const hardware::buffer>> read_only_operands,
		hardware::device_queue *queue
	) const override;

private:
	array_access_layout m_access_layout;
	numerical_type m_output_data_type;
	numerical_type m_input_data_type;
};

} // namespace multidimensional
} // namespace xmipp4
