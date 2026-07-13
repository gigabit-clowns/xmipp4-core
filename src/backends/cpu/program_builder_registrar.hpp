// SPDX-License-Identifier: GPL-3.0-only

#pragma once

namespace xmipp4
{

class program_manager;

namespace cpu
{

/**
 * @brief Register all CPU operation programs bundled in the core to a
 * @ref program_manager.
 *
 * @param manager The program builder manager where programs are registered.
 */
void register_program_builders(xmipp4::program_manager &manager);

} // namespace cpu
} // namespace xmipp4

