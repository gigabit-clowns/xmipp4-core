// SPDX-License-Identifier: GPL-3.0-only

#include "cpu_kernel_registrar.hpp"

#include <xmipp4/core/multidimensional/kernel_manager.hpp>

#include "cpu_copy_kernel_builder.hpp"
#include "cpu_fill_kernel_builder.hpp"

namespace xmipp4 
{
namespace multidimensional
{

void register_cpu_kernels(kernel_manager &manager)
{
	manager.register_kernel(std::make_unique<cpu_copy_kernel_builder>());
	manager.register_kernel(std::make_unique<cpu_fill_kernel_builder>());
}

} // namespace multidimensional
} // namespace xmipp4

