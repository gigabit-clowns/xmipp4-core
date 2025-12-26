// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "cpu_kernel.hpp"

#include <xmipp4/core/multidimensional/array_access_layout.hpp>

namespace xmipp4 
{
namespace multidimensional
{

template <typename T, typename Q>
class cpu_copy_kernel final
	: public cpu_kernel
{
public:
	using output_value_type = T;
	using input_value_type = Q;

	explicit cpu_copy_kernel(array_access_layout access_layout) noexcept;
	~cpu_copy_kernel() override = default;

	void execute(
		span<const std::shared_ptr<hardware::buffer>> read_write_operands,
		span<const std::shared_ptr<const hardware::buffer>> read_only_operands,
		hardware::device_queue *queue
	) const override;

private:
	array_access_layout m_access_layout;
};

} // namespace multidimensional
} // namespace xmipp4

#include "cpu_copy_kernel.inl"
