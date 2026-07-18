// SPDX-License-Identifier: GPL-3.0-only

#include "program_builder_registrar.hpp"

#include <xmipp4/core/dispatch/program_manager.hpp>

#include "builders/arithmetic/abs_program_builder.hpp"
#include "builders/arithmetic/divide_program_builder.hpp"
#include "builders/arithmetic/modulo_program_builder.hpp"
#include "builders/arithmetic/multiply_program_builder.hpp"
#include "builders/arithmetic/negate_program_builder.hpp"
#include "builders/arithmetic/subtract_program_builder.hpp"
#include "builders/assignment/copy_program_builder.hpp"
#include "builders/assignment/fill_program_builder.hpp"

namespace xmipp4
{
namespace cpu
{

void register_program_builders(xmipp4::program_manager &manager)
{
	manager.register_builder(
		std::make_unique<copy_program_builder>()
	);
	manager.register_builder(
		std::make_unique<fill_program_builder>()
	);
	manager.register_builder(
		std::make_unique<subtract_program_builder>()
	);
	manager.register_builder(
		std::make_unique<multiply_program_builder>()
	);
	manager.register_builder(
		std::make_unique<divide_program_builder>()
	);
	manager.register_builder(
		std::make_unique<negate_program_builder>()
	);
	manager.register_builder(
		std::make_unique<modulo_program_builder>()
	);
	manager.register_builder(
		std::make_unique<abs_program_builder>()
	);
}

} // namespace cpu
} // namespace xmipp4
