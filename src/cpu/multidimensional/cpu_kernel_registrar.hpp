// SPDX-License-Identifier: GPL-3.0-only

#pragma once

namespace xmipp4 
{
namespace multidimensional
{

class kernel_manager;

/**
 * @brief Register all CPU kernels bundled in the core in a kernel manager.
 * 
 * @param manager The kernel manager where kernels are registered.
 */
void register_cpu_kernels(kernel_manager &manager);

} // namespace multidimensional
} // namespace xmipp4

