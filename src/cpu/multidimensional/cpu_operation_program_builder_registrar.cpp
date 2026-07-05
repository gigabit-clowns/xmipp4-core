// SPDX-License-Identifier: GPL-3.0-only

#include "cpu_operation_program_builder_registrar.hpp"

#include <xmipp4/core/multidimensional/operation_program_manager.hpp>

#include "cpu_operation_program_builders/assignment/cpu_copy_operation_program_builder.hpp"
#include "cpu_operation_program_builders/assignment/cpu_fill_operation_program_builder.hpp"

namespace xmipp4 
{
namespace multidimensional
{

void register_cpu_operation_program_builders(operation_program_manager &manager)
{
	manager.register_builder(
		std::make_unique<cpu_copy_operation_program_builder>()
	);
	manager.register_builder(
		std::make_unique<cpu_fill_operation_program_builder>()
	);
}

} // namespace multidimensional
} // namespace xmipp4
