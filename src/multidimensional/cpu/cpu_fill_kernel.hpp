// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "cpu_kernel.hpp"

#include <xmipp4/core/multidimensional/array_access_layout.hpp>

namespace xmipp4 
{
namespace multidimensional
{

template <typename T>
class cpu_fill_kernel final
	: public cpu_kernel
{
public:
	using output_value_type = T;

	cpu_fill_kernel(
		array_access_layout access_layout,
		output_value_type fill_value
	) noexcept;
	~cpu_fill_kernel() override = default;

	void execute(
		span<const std::shared_ptr<hardware::buffer>> read_write_operands,
		span<const std::shared_ptr<const hardware::buffer>> read_only_operands,
		hardware::device_queue *queue
	) const override;

private:
	array_access_layout m_access_layout;
	output_value_type m_fill_value;

	void fill(output_value_type *destination) const;
};

} // namespace multidimensional
} // namespace xmipp4

#include "cpu_fill_kernel.inl"
