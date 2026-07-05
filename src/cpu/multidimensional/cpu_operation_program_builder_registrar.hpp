// SPDX-License-Identifier: GPL-3.0-only

#pragma once

namespace xmipp4 
{
namespace multidimensional
{

class operation_program_manager;

/**
 * @brief Register all CPU operation programs bundled in the core to a 
 * @ref operation_program_manager.
 * 
 * @param manager The program builder manager where programs are registered.
 */
void register_cpu_operation_program_builders(operation_program_manager &manager);

} // namespace multidimensional
} // namespace xmipp4

