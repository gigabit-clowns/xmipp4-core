// SPDX-License-Identifier: GPL-3.0-only

#include <catch2/catch_test_macros.hpp>

#include <xmipp4/core/multidimensional/operation_command_manager.hpp>
#include <xmipp4/core/multidimensional/operation_command_cache.hpp>

#include <xmipp4/core/exceptions/invalid_operation_error.hpp>

#include "mock/mock_operation.hpp"
#include "mock/mock_operation_command_builder.hpp"
#include "../hardware/mock/mock_command.hpp"

#include <array>

using namespace xmipp4;
using namespace xmipp4::multidimensional;

namespace
{

struct mock_operation_a
	: mock_operation
{
	~mock_operation_a() override = default;
};

struct mock_operation_b
	: mock_operation
{
	~mock_operation_b() override = default;
};

} // namespace

TEST_CASE(
	"operation_command_manager::register_builder accepts a valid builder",
	"[operation_command_manager]"
)
{
	operation_command_manager manager;
	auto builder = std::make_unique<mock_operation_command_builder>();

	REQUIRE_CALL(*builder, get_operation_id())
		.RETURN(operation_id::of<mock_operation_command_builder>());

	CHECK(manager.register_builder(std::move(builder)));
}

TEST_CASE(
	"operation_command_manager::register_builder rejects a null builder",
	"[operation_command_manager]"
)
{
	operation_command_manager manager;
	CHECK_FALSE(manager.register_builder(nullptr));
}

TEST_CASE(
	"operation_command_manager::register_builder accepts multiple builders "
	"for the same operation",
	"[operation_command_manager]"
)
{
	operation_command_manager manager;
	auto builder1 = std::make_unique<mock_operation_command_builder>();
	auto builder2 = std::make_unique<mock_operation_command_builder>();

	REQUIRE_CALL(*builder1, get_operation_id())
		.RETURN(operation_id::of<mock_operation_command_builder>());
	REQUIRE_CALL(*builder2, get_operation_id())
		.RETURN(operation_id::of<mock_operation_command_builder>());

	CHECK(manager.register_builder(std::move(builder1)));
	CHECK(manager.register_builder(std::move(builder2)));
}

TEST_CASE(
	"operation_command_manager::build dispatches to the builder with the "
	"highest suitability",
	"[operation_command_manager]"
)
{
	operation_command_manager manager;

	const mock_operation_a op;
	const std::array<array_signature, 2> output_signatures;
	const std::array<array_signature, 4> input_signatures;

	auto builder1 = std::make_unique<mock_operation_command_builder>();
	auto builder2 = std::make_unique<mock_operation_command_builder>();
	const auto cmd = std::make_shared<hardware::mock_command>();

	REQUIRE_CALL(*builder1, get_operation_id())
		.RETURN(operation_id::of<mock_operation_a>());
	REQUIRE_CALL(
		*builder1,
		get_suitability(
			ANY(const operation&),
			ANY(span<const array_signature>),
			ANY(span<const array_signature>)
		)
	)
		.RETURN(backend_priority::fallback);

	REQUIRE_CALL(*builder2, get_operation_id())
		.RETURN(operation_id::of<mock_operation_a>());
	REQUIRE_CALL(
		*builder2,
		get_suitability(
			ANY(const operation&),
			ANY(span<const array_signature>),
			ANY(span<const array_signature>)
		)
	)
		.RETURN(backend_priority::normal);
	REQUIRE_CALL(
		*builder2,
		build(
			ANY(const operation&),
			ANY(span<const array_signature>),
			ANY(span<const array_signature>),
			ANY(operation_command_cache*)
		)
	)
		.RETURN(cmd);

	REQUIRE(manager.register_builder(std::move(builder1)));
	REQUIRE(manager.register_builder(std::move(builder2)));

	CHECK(
		manager.build(
			op,
			make_span(output_signatures),
			make_span(input_signatures)
		) == cmd
	);
}

TEST_CASE(
	"operation_command_manager::build filters builders by operation id",
	"[operation_command_manager]"
)
{
	operation_command_manager manager;

	const mock_operation_a op;
	const std::array<array_signature, 2> output_signatures;
	const std::array<array_signature, 4> input_signatures;

	auto builder_a = std::make_unique<mock_operation_command_builder>();
	auto builder_b = std::make_unique<mock_operation_command_builder>();
	const auto cmd = std::make_shared<hardware::mock_command>();

	REQUIRE_CALL(*builder_a, get_operation_id())
		.RETURN(operation_id::of<mock_operation_a>());
	REQUIRE_CALL(
		*builder_a,
		get_suitability(
			ANY(const operation&),
			ANY(span<const array_signature>),
			ANY(span<const array_signature>)
		)
	)
		.RETURN(backend_priority::normal);
	REQUIRE_CALL(
		*builder_a,
		build(
			ANY(const operation&),
			ANY(span<const array_signature>),
			ANY(span<const array_signature>),
			ANY(operation_command_cache*)
		)
	)
		.RETURN(cmd);

	// Registered for a different op id: must not be consulted.
	REQUIRE_CALL(*builder_b, get_operation_id())
		.RETURN(operation_id::of<mock_operation_b>());

	REQUIRE(manager.register_builder(std::move(builder_a)));
	REQUIRE(manager.register_builder(std::move(builder_b)));

	CHECK(
		manager.build(
			op,
			make_span(output_signatures),
			make_span(input_signatures)
		) == cmd
	);
}

TEST_CASE(
	"operation_command_manager::build forwards a null cache by default",
	"[operation_command_manager]"
)
{
	operation_command_manager manager;

	const mock_operation_a op;
	const std::array<array_signature, 2> output_signatures;
	const std::array<array_signature, 4> input_signatures;

	auto builder = std::make_unique<mock_operation_command_builder>();
	const auto cmd = std::make_shared<hardware::mock_command>();

	REQUIRE_CALL(*builder, get_operation_id())
		.RETURN(operation_id::of<mock_operation_a>());
	REQUIRE_CALL(
		*builder,
		get_suitability(
			ANY(const operation&),
			ANY(span<const array_signature>),
			ANY(span<const array_signature>)
		)
	)
		.RETURN(backend_priority::normal);
	REQUIRE_CALL(
		*builder,
		build(
			ANY(const operation&),
			ANY(span<const array_signature>),
			ANY(span<const array_signature>),
			ANY(operation_command_cache*)
		)
	)
		.LR_WITH(_4 == nullptr)
		.RETURN(cmd);

	REQUIRE(manager.register_builder(std::move(builder)));

	CHECK(
		manager.build(
			op,
			make_span(output_signatures),
			make_span(input_signatures)
		) == cmd
	);
}

TEST_CASE(
	"operation_command_manager::build forwards the cache pointer to the "
	"selected builder",
	"[operation_command_manager]"
)
{
	operation_command_manager manager;

	const mock_operation_a op;
	const std::array<array_signature, 2> output_signatures;
	const std::array<array_signature, 4> input_signatures;
	operation_command_cache cache(4);

	auto builder = std::make_unique<mock_operation_command_builder>();
	const auto cmd = std::make_shared<hardware::mock_command>();

	REQUIRE_CALL(*builder, get_operation_id())
		.RETURN(operation_id::of<mock_operation_a>());
	REQUIRE_CALL(
		*builder,
		get_suitability(
			ANY(const operation&),
			ANY(span<const array_signature>),
			ANY(span<const array_signature>)
		)
	)
		.RETURN(backend_priority::normal);
	REQUIRE_CALL(
		*builder,
		build(
			ANY(const operation&),
			ANY(span<const array_signature>),
			ANY(span<const array_signature>),
			ANY(operation_command_cache*)
		)
	)
		.LR_WITH(_4 == &cache)
		.RETURN(cmd);

	REQUIRE(manager.register_builder(std::move(builder)));

	CHECK(
		manager.build(
			op,
			make_span(output_signatures),
			make_span(input_signatures),
			&cache
		) == cmd
	);
}

TEST_CASE(
	"operation_command_manager::build throws when no builder handles the "
	"operation",
	"[operation_command_manager]"
)
{
	operation_command_manager manager;

	const mock_operation_b op;
	const std::array<array_signature, 2> output_signatures;
	const std::array<array_signature, 4> input_signatures;

	auto builder = std::make_unique<mock_operation_command_builder>();
	REQUIRE_CALL(*builder, get_operation_id())
		.RETURN(operation_id::of<mock_operation_a>());

	REQUIRE(manager.register_builder(std::move(builder)));

	REQUIRE_THROWS_AS(
		manager.build(
			op,
			make_span(output_signatures),
			make_span(input_signatures)
		),
		invalid_operation_error
	);
}

TEST_CASE(
	"operation_command_manager::build throws when every candidate reports "
	"unsupported",
	"[operation_command_manager]"
)
{
	operation_command_manager manager;

	const mock_operation_a op;
	const std::array<array_signature, 2> output_signatures;
	const std::array<array_signature, 4> input_signatures;

	auto builder1 = std::make_unique<mock_operation_command_builder>();
	auto builder2 = std::make_unique<mock_operation_command_builder>();

	REQUIRE_CALL(*builder1, get_operation_id())
		.RETURN(operation_id::of<mock_operation_a>());
	REQUIRE_CALL(
		*builder1,
		get_suitability(
			ANY(const operation&),
			ANY(span<const array_signature>),
			ANY(span<const array_signature>)
		)
	)
		.RETURN(backend_priority::unsupported);

	REQUIRE_CALL(*builder2, get_operation_id())
		.RETURN(operation_id::of<mock_operation_a>());
	REQUIRE_CALL(
		*builder2,
		get_suitability(
			ANY(const operation&),
			ANY(span<const array_signature>),
			ANY(span<const array_signature>)
		)
	)
		.RETURN(backend_priority::unsupported);

	REQUIRE(manager.register_builder(std::move(builder1)));
	REQUIRE(manager.register_builder(std::move(builder2)));

	REQUIRE_THROWS_AS(
		manager.build(
			op,
			make_span(output_signatures),
			make_span(input_signatures)
		),
		invalid_operation_error
	);
}
