// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/multidimensional/array_descriptor.hpp>
#include <xmipp4/core/multidimensional/operation_id.hpp>
#include <xmipp4/core/multidimensional/kernel.hpp>
#include <xmipp4/core/multidimensional/kernel_manager.hpp>

#include <memory>
#include <unordered_map>

#include "kernel_cache_key.hpp"

namespace xmipp4 
{
namespace multidimensional
{

class kernel_cache
{
public:
	const std::shared_ptr<kernel>& get_kernel(
		const operation &op,
		span<const array_descriptor> output_descriptors,
		span<const array_descriptor> input_descriptors,
		const hardware::device &device,
		const kernel_manager &manager
	);

private:
	std::unordered_map<kernel_cache_key, std::shared_ptr<kernel>> m_items;
};

} // namespace multidimensional
} // namespace xmipp4
