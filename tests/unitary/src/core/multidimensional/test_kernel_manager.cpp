// SPDX-License-Identifier: GPL-3.0-only

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_exception.hpp>

#include <xmipp4/core/multidimensional/kernel_manager.hpp>

#include <xmipp4/core/exceptions/invalid_operation_error.hpp>

#include "mock/mock_operation.hpp"
#include "mock/mock_kernel_builder.hpp"
#include "mock/mock_kernel.hpp"
#include "../hardware/mock/mock_device.hpp"

#include <array>

using namespace xmipp4;
using namespace xmipp4::multidimensional;

TEST_CASE("register_kernel in kernel_manager should return true with a valid kernel_builder", "[kernel_manager]")
{
	kernel_manager manager;

	auto builder = std::make_unique<mock_kernel_builder>();
	
	REQUIRE_CALL(*builder, get_operation_id())
		.RETURN(operation_id::of<mock_kernel_builder>());

	CHECK( manager.register_kernel(std::move(builder)) );
}

TEST_CASE("register_kernel in kernel_manager should return false with an invalid kernel_builder", "[kernel_manager]")
{
	kernel_manager manager;

	CHECK( manager.register_kernel(nullptr) == false );
}

TEST_CASE("register_kernel in kernel_manager should allow registering multiple kernels for the same operation", "[kernel_manager]")
{
	kernel_manager manager;

	auto builder1 = std::make_unique<mock_kernel_builder>();
	auto builder2 = std::make_unique<mock_kernel_builder>();
	REQUIRE_CALL(*builder1, get_operation_id())
		.RETURN(operation_id::of<mock_kernel_builder>());
	REQUIRE_CALL(*builder2, get_operation_id())
		.RETURN(operation_id::of<mock_kernel_builder>());

	CHECK( manager.register_kernel(std::move(builder1)) );
	CHECK( manager.register_kernel(std::move(builder2)) );
}

TEST_CASE("build in kernel_manager should call build on the most appropiate kernel_builder", "[kernel_manager]")
{
	kernel_manager manager;

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
	const std::array<array_descriptor, 4> descriptors;
	hardware::mock_device device;

	auto builder1 = std::make_unique<mock_kernel_builder>();
	auto builder2 = std::make_unique<mock_kernel_builder>();
	auto builder3 = std::make_unique<mock_kernel_builder>();
	auto kernel = std::make_shared<mock_kernel>();
	REQUIRE_CALL(*builder1, get_operation_id())
		.RETURN(operation_id::of<mock_operation1>());
	REQUIRE_CALL(
		*builder1, 
		get_suitability(
			ANY(const operation&), 
			ANY(span<const array_descriptor>),
			ANY(hardware::device&)
		)
	)
		.LR_WITH(&_1 == &op)
		.LR_WITH(_2.data() == descriptors.data() && _2.size() == descriptors.size())
		.LR_WITH(&_3 == &device)
		.RETURN(backend_priority::fallback);

	REQUIRE_CALL(*builder2, get_operation_id())
		.RETURN(operation_id::of<mock_operation1>());
	REQUIRE_CALL(
		*builder2, 
		get_suitability(
			ANY(const operation&), 
			ANY(span<const array_descriptor>),
			ANY(hardware::device&)
		)
	)
		.LR_WITH(&_1 == &op)
		.LR_WITH(_2.data() == descriptors.data() && _2.size() == descriptors.size())
		.LR_WITH(&_3 == &device)
		.RETURN(backend_priority::normal);
	REQUIRE_CALL(
		*builder2, 
		build(
			ANY(const operation&), 
			ANY(span<const array_descriptor>),
			ANY(hardware::device&)
		)
	)
		.LR_WITH(&_1 == &op)
		.LR_WITH(_2.data() == descriptors.data() && _2.size() == descriptors.size())
		.LR_WITH(&_3 == &device)
		.RETURN(kernel);

	REQUIRE_CALL(*builder3, get_operation_id())
		.RETURN(operation_id::of<mock_operation2>());

	REQUIRE( manager.register_kernel(std::move(builder1)) );
	REQUIRE( manager.register_kernel(std::move(builder2)) );
	REQUIRE( manager.register_kernel(std::move(builder3)) );

	auto result = manager.build_kernel(op, make_span(descriptors), device);
	CHECK( result == kernel );
}

TEST_CASE("build in kernel_manager should throw if no builder is supported", "[kernel_manager]")
{
	kernel_manager manager;

	const mock_operation op;
	const std::array<array_descriptor, 4> descriptors;
	hardware::mock_device device;

	auto builder1 = std::make_unique<mock_kernel_builder>();
	auto builder2 = std::make_unique<mock_kernel_builder>();
	auto kernel = std::make_shared<mock_kernel>();
	REQUIRE_CALL(*builder1, get_operation_id())
		.RETURN(operation_id::of<mock_operation>());
	REQUIRE_CALL(
		*builder1, 
		get_suitability(
			ANY(const operation&), 
			ANY(span<const array_descriptor>),
			ANY(hardware::device&)
		)
	)
		.LR_WITH(&_1 == &op)
		.LR_WITH(_2.data() == descriptors.data() && _2.size() == descriptors.size())
		.LR_WITH(&_3 == &device)
		.RETURN(backend_priority::unsupported);

	REQUIRE_CALL(*builder2, get_operation_id())
		.RETURN(operation_id::of<mock_operation>());
	REQUIRE_CALL(
		*builder2, 
		get_suitability(
			ANY(const operation&), 
			ANY(span<const array_descriptor>),
			ANY(hardware::device&)
		)
	)
		.LR_WITH(&_1 == &op)
		.LR_WITH(_2.data() == descriptors.data() && _2.size() == descriptors.size())
		.LR_WITH(&_3 == &device)
		.RETURN(backend_priority::unsupported);

	REQUIRE( manager.register_kernel(std::move(builder1)) );
	REQUIRE( manager.register_kernel(std::move(builder2)) );

	REQUIRE_THROWS_MATCHES(
		manager.build_kernel(op, make_span(descriptors), device),
		invalid_operation_error,
		Catch::Matchers::Message(
			"Could not find a suitable kernel builder for the requested "
			"operation"
		)
	);
}

TEST_CASE("build in kernel_manager should throw if there are no backends for the requested operation", "[kernel_manager]")
{
	kernel_manager manager;

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
	const std::array<array_descriptor, 4> descriptors;
	hardware::mock_device device;

	auto builder = std::make_unique<mock_kernel_builder>();
	auto kernel = std::make_shared<mock_kernel>();
	REQUIRE_CALL(*builder, get_operation_id())
		.RETURN(operation_id::of<mock_operation1>());

	REQUIRE( manager.register_kernel(std::move(builder)) );

	REQUIRE_THROWS_MATCHES(
		manager.build_kernel(op, make_span(descriptors), device),
		invalid_operation_error,
		Catch::Matchers::Message(
			"Could not find a suitable kernel builder for the requested "
			"operation"
		)
	);
}
