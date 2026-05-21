// SPDX-License-Identifier: GPL-3.0-only

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_exception.hpp>

#include <xmipp4/core/multidimensional/operation_command_manager.hpp>

#include <xmipp4/core/exceptions/invalid_operation_error.hpp>

#include "mock/mock_operation.hpp"
#include "mock/mock_operation_command_builder.hpp"
#include "../hardware/mock/mock_command.hpp"
#include "../hardware/mock/mock_device.hpp"

#include <array>

using namespace xmipp4;
using namespace xmipp4::multidimensional;

TEST_CASE("register_operation_command in operation_command_manager should return true with a valid operation command builder", "[operation_command_manager]")
{
	operation_command_manager manager;

	auto builder = std::make_unique<mock_operation_command_builder>();
	
	REQUIRE_CALL(*builder, get_operation_id())
		.RETURN(operation_id::of<mock_operation_command_builder>());

	CHECK( manager.register_operation_command(std::move(builder)) );
}

TEST_CASE("register_operation_command in operation_command_manager should return false with an invalid operation command builder", "[operation_command_manager]")
{
	operation_command_manager manager;

	CHECK( manager.register_operation_command(nullptr) == false );
}

TEST_CASE("register_operation_command in operation_command_manager should allow registering multiple operation commands for the same operation", "[operation_command_manager]")
{
	operation_command_manager manager;

	auto builder1 = std::make_unique<mock_operation_command_builder>();
	auto builder2 = std::make_unique<mock_operation_command_builder>();
	REQUIRE_CALL(*builder1, get_operation_id())
		.RETURN(operation_id::of<mock_operation_command_builder>());
	REQUIRE_CALL(*builder2, get_operation_id())
		.RETURN(operation_id::of<mock_operation_command_builder>());

	CHECK( manager.register_operation_command(std::move(builder1)) );
	CHECK( manager.register_operation_command(std::move(builder2)) );
}

TEST_CASE("build_operation_command in operation_command_manager should call build on the most appropiate operation command builder", "[operation_command_manager]")
{
	operation_command_manager manager;

	struct mock_operation1
		: mock_operation
	{
		~mock_operation1() override = default;
	};
	struct mock_operation2
		: mock_operation
	{
		~mock_operation2() override = default;
	};
	
	const mock_operation1 op;
	const std::array<array_signature, 2> output_signatures;
	const std::array<array_signature, 4> input_signatures;

	auto builder1 = std::make_unique<mock_operation_command_builder>();
	auto builder2 = std::make_unique<mock_operation_command_builder>();
	auto builder3 = std::make_unique<mock_operation_command_builder>();
	auto kernel = std::make_shared<hardware::mock_command>();
	REQUIRE_CALL(*builder1, get_operation_id())
		.RETURN(operation_id::of<mock_operation1>());
	REQUIRE_CALL(
		*builder1, 
		get_suitability(
			ANY(const operation&), 
			ANY(span<const array_signature>),
			ANY(span<const array_signature>)
		)
	)
		.LR_WITH(&_1 == &op)
		.LR_WITH(_2.data() == output_signatures.data() && _2.size() == output_signatures.size())
		.LR_WITH(_3.data() == input_signatures.data() && _3.size() == input_signatures.size())
		.RETURN(backend_priority::fallback);

	REQUIRE_CALL(*builder2, get_operation_id())
		.RETURN(operation_id::of<mock_operation1>());
	REQUIRE_CALL(
		*builder2, 
		get_suitability(
			ANY(const operation&), 
			ANY(span<const array_signature>),
			ANY(span<const array_signature>)
		)
	)
		.LR_WITH(&_1 == &op)
		.LR_WITH(_2.data() == output_signatures.data() && _2.size() == output_signatures.size())
		.LR_WITH(_3.data() == input_signatures.data() && _3.size() == input_signatures.size())
		.RETURN(backend_priority::normal);
	REQUIRE_CALL(
		*builder2, 
		build(
			ANY(const operation&), 
			ANY(span<const array_signature>),
			ANY(span<const array_signature>)
		)
	)
		.LR_WITH(&_1 == &op)
		.LR_WITH(_2.data() == output_signatures.data() && _2.size() == output_signatures.size())
		.LR_WITH(_3.data() == input_signatures.data() && _3.size() == input_signatures.size())
		.RETURN(kernel);

	REQUIRE_CALL(*builder3, get_operation_id())
		.RETURN(operation_id::of<mock_operation2>());

	REQUIRE( manager.register_operation_command(std::move(builder1)) );
	REQUIRE( manager.register_operation_command(std::move(builder2)) );
	REQUIRE( manager.register_operation_command(std::move(builder3)) );

	auto result = manager.build_operation_command(op, make_span(output_signatures), make_span(input_signatures));
	;
	CHECK( result == kernel );
}

TEST_CASE("build_operation_command in operation_command_manager should throw if no builder is supported", "[operation_command_manager]")
{
	operation_command_manager manager;

	const mock_operation op;
	const std::array<array_signature, 2> output_signatures;
	const std::array<array_signature, 4> input_signatures;

	auto builder1 = std::make_unique<mock_operation_command_builder>();
	auto builder2 = std::make_unique<mock_operation_command_builder>();
	auto kernel = std::make_shared<hardware::mock_command>();
	REQUIRE_CALL(*builder1, get_operation_id())
		.RETURN(operation_id::of<mock_operation>());
	REQUIRE_CALL(
		*builder1, 
		get_suitability(
			ANY(const operation&), 
			ANY(span<const array_signature>),
			ANY(span<const array_signature>)
		)
	)
		.LR_WITH(&_1 == &op)
		.LR_WITH(_2.data() == output_signatures.data() && _2.size() == output_signatures.size())
		.LR_WITH(_3.data() == input_signatures.data() && _3.size() == input_signatures.size())
		.RETURN(backend_priority::unsupported);

	REQUIRE_CALL(*builder2, get_operation_id())
		.RETURN(operation_id::of<mock_operation>());
	REQUIRE_CALL(
		*builder2, 
		get_suitability(
			ANY(const operation&), 
			ANY(span<const array_signature>),
			ANY(span<const array_signature>)
		)
	)
		.LR_WITH(&_1 == &op)
		.LR_WITH(_2.data() == output_signatures.data() && _2.size() == output_signatures.size())
		.LR_WITH(_3.data() == input_signatures.data() && _3.size() == input_signatures.size())
		.RETURN(backend_priority::unsupported);

	REQUIRE( manager.register_operation_command(std::move(builder1)) );
	REQUIRE( manager.register_operation_command(std::move(builder2)) );

	REQUIRE_THROWS_MATCHES(
		manager.build_operation_command(op, make_span(output_signatures), make_span(input_signatures)),
		invalid_operation_error,
		Catch::Matchers::Message(
			"Could not find a suitable operation command builder for the requested "
			"operation"
		)
	);
}

TEST_CASE("build_operation_command in operation_command_manager should throw if there are no backends for the requested operation", "[operation_command_manager]")
{
	operation_command_manager manager;

	struct mock_operation1
		: mock_operation
	{
		~mock_operation1() override = default;
	};
	struct mock_operation2
		: mock_operation
	{
		~mock_operation2() override = default;
	};

	const mock_operation2 op;
	const std::array<array_signature, 2> output_signatures;
	const std::array<array_signature, 4> input_signatures;

	auto builder = std::make_unique<mock_operation_command_builder>();
	auto kernel = std::make_shared<hardware::mock_command>();
	REQUIRE_CALL(*builder, get_operation_id())
		.RETURN(operation_id::of<mock_operation1>());

	REQUIRE( manager.register_operation_command(std::move(builder)) );

	REQUIRE_THROWS_MATCHES(
		manager.build_operation_command(op, make_span(output_signatures), make_span(input_signatures)),
		invalid_operation_error,
		Catch::Matchers::Message(
			"Could not find a suitable operation command builder for the requested "
			"operation"
		)
	);
}
