// SPDX-License-Identifier: GPL-3.0-only

#pragma once

namespace xmipp4
{
namespace dispatch
{

class program_manager;

/**
 * @brief Register all CPU operation programs bundled in the core to a
 * @ref program_manager.
 *
 * @param manager The program builder manager where programs are registered.
 */
void register_cpu_program_builders(program_manager &manager);

} // namespace dispatch
} // namespace xmipp4

