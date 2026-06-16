// SPDX-License-Identifier: GPL-3.0-only

#include <catch2/catch_test_macros.hpp>

#include <xmipp4/core/multidimensional/operation_dispatcher.hpp>

#include <xmipp4/core/multidimensional/operation_command_manager.hpp>

#include <memory>
#include <stdexcept>

using namespace xmipp4;
using namespace xmipp4::multidimensional;

TEST_CASE(
	"make_eager_operation_dispatcher builds a non-null dispatcher",
	"[operation_dispatcher]"
)
{
	const auto command_manager = std::make_shared<operation_command_manager>();

	const auto dispatcher = make_eager_operation_dispatcher(command_manager);

	CHECK( dispatcher != nullptr );
}

TEST_CASE(
	"make_eager_operation_dispatcher throws when the command manager is null",
	"[operation_dispatcher]"
)
{
	CHECK_THROWS_AS(
		make_eager_operation_dispatcher(
			std::shared_ptr<const operation_command_manager>()
		),
		std::invalid_argument
	);
}
