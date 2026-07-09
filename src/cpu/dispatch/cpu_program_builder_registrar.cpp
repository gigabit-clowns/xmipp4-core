// SPDX-License-Identifier: GPL-3.0-only

#include "cpu_program_builder_registrar.hpp"

#include <xmipp4/core/dispatch/program_manager.hpp>

#include "builders/assignment/cpu_copy_program_builder.hpp"
#include "builders/assignment/cpu_fill_program_builder.hpp"

namespace xmipp4
{
namespace dispatch
{

void register_cpu_program_builders(program_manager &manager)
{
	manager.register_builder(
		std::make_unique<cpu_copy_program_builder>()
	);
	manager.register_builder(
		std::make_unique<cpu_fill_program_builder>()
	);
}

} // namespace dispatch
} // namespace xmipp4
