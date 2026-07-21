// SPDX-License-Identifier: GPL-3.0-only

#include <catch2/catch_test_macros.hpp>

#include <xmipp4/functional/creation.hpp>

#include "fixtures/cpu_execution_context_fixture.hpp"

#include <xmipp4/core/ndarray/array.hpp>
#include <xmipp4/core/ndarray/const_array.hpp>
#include <xmipp4/core/hardware/memory_resource_affinity.hpp>
#include <xmipp4/core/numerical/scalar_value.hpp>

#include <vector>

using namespace xmipp4;

TEST_CASE_METHOD(
	cpu_execution_context_fixture,
	"empty produces a host-accessible array carrying the requested descriptor",
	"[array_creation][cpu]"
)
{
	const auto descriptor = make_descriptor({ 2, 3 });

	const auto result = empty(
		descriptor,
		memory_resource_affinity::host,
		context
	);

	CHECK( result.get_descriptor() == descriptor );

	// The storage must exist and be reachable from the host, even though its
	// contents are left uninitialized.
	const auto *storage = result.get_storage();
	REQUIRE( storage != nullptr );
	CHECK( storage->get_host_ptr() != nullptr );
}

TEST_CASE_METHOD(
	cpu_execution_context_fixture,
	"zeros sets every element of a host array to zero",
	"[array_creation][cpu]"
)
{
	const auto descriptor = make_descriptor({ 2, 3 });

	const auto result = zeros(
		descriptor,
		memory_resource_affinity::host,
		context
	);

	CHECK( result.get_descriptor() == descriptor );

	const auto values = read_host<float>(result, 6);
	for (const auto value : values)
	{
		CHECK( value == 0.0f );
	}
}

TEST_CASE_METHOD(
	cpu_execution_context_fixture,
	"ones sets every element of a host array to one",
	"[array_creation][cpu]"
)
{
	const auto descriptor = make_descriptor({ 2, 3 });

	const auto result = ones(
		descriptor,
		memory_resource_affinity::host,
		context
	);

	CHECK( result.get_descriptor() == descriptor );

	const auto values = read_host<float>(result, 6);
	for (const auto value : values)
	{
		CHECK( value == 1.0f );
	}
}

TEST_CASE_METHOD(
	cpu_execution_context_fixture,
	"full sets every element of a host array to the requested value",
	"[array_creation][cpu]"
)
{
	const auto descriptor = make_descriptor({ 4 });

	const auto result = full(
		descriptor,
		memory_resource_affinity::host,
		scalar_value(3.5f),
		context
	);

	const auto values = read_host<float>(result, 4);
	for (const auto value : values)
	{
		CHECK( value == 3.5f );
	}
}

TEST_CASE_METHOD(
	cpu_execution_context_fixture,
	"fill overwrites the contents of an existing host array",
	"[array_creation][cpu]"
)
{
	const auto descriptor = make_descriptor({ 5 });

	// Start from zeros so a successful fill is unambiguous.
	auto result = zeros(
		descriptor,
		memory_resource_affinity::host,
		context
	);

	fill(result, scalar_value(7.0f), context);

	const auto values = read_host<float>(result, 5);
	for (const auto value : values)
	{
		CHECK( value == 7.0f );
	}
}

TEST_CASE_METHOD(
	cpu_execution_context_fixture,
	"copy duplicates the source contents into independent storage",
	"[array_creation][cpu]"
)
{
	const auto descriptor = make_descriptor({ 2, 3 });

	const auto source = full(
		descriptor,
		memory_resource_affinity::host,
		scalar_value(2.0f),
		context
	);

	const auto result = copy(source, context);

	CHECK( result.get_descriptor() == descriptor );

	// The copy owns a distinct buffer from the source.
	CHECK( result.get_storage() != source.get_storage() );

	const auto values = read_host<float>(result, 6);
	for (const auto value : values)
	{
		CHECK( value == 2.0f );
	}
}
