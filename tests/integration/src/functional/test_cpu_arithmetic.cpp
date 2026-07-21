// SPDX-License-Identifier: GPL-3.0-only

#include <catch2/catch_test_macros.hpp>

#include <xmipp4/functional/arithmetic.hpp>

#include "fixtures/cpu_execution_context_fixture.hpp"

#include <xmipp4/functional/creation.hpp>
#include <xmipp4/core/ndarray/array.hpp>
#include <xmipp4/core/ndarray/const_array.hpp>
#include <xmipp4/core/ndarray/const_array_ref.hpp>
#include <xmipp4/core/hardware/memory_resource_affinity.hpp>
#include <xmipp4/core/numerical/scalar_value.hpp>

#include <vector>

using namespace xmipp4;

TEST_CASE_METHOD(
	cpu_execution_context_fixture,
	"add computes the element-wise sum of two arrays",
	"[array_arithmetic][cpu]"
)
{
	const auto descriptor = make_descriptor({ 2, 3 });

	auto x = full(
		descriptor, memory_resource_affinity::device, scalar_value(2.0f), context
	);
	auto y = full(
		descriptor, memory_resource_affinity::device, scalar_value(3.0f), context
	);
	const_array_ref x_ref = x;
	const_array_ref y_ref = y;

	const auto result = add(x_ref, y_ref, context, nullptr);

	CHECK( result.get_descriptor() == descriptor );
	for (const auto value : read_host<float>(result, 6))
	{
		CHECK( value == 5.0f );
	}
}

TEST_CASE_METHOD(
	cpu_execution_context_fixture,
	"add reuses the storage of the provided output array",
	"[array_arithmetic][cpu]"
)
{
	const auto descriptor = make_descriptor({ 2, 3 });

	auto x = full(
		descriptor, memory_resource_affinity::device, scalar_value(2.0f), context
	);
	auto y = full(
		descriptor, memory_resource_affinity::device, scalar_value(3.0f), context
	);
	const_array_ref x_ref = x;
	const_array_ref y_ref = y;

	auto out = zeros(descriptor, memory_resource_affinity::device, context);
	const auto *out_storage = out.get_storage();

	const auto result = add(x_ref, y_ref, context, &out);

	CHECK( result.get_storage() == out_storage );
	for (const auto value : read_host<float>(result, 6))
	{
		CHECK( value == 5.0f );
	}
}

TEST_CASE_METHOD(
	cpu_execution_context_fixture,
	"subtract computes the element-wise difference of two arrays",
	"[array_arithmetic][cpu]"
)
{
	const auto descriptor = make_descriptor({ 2, 3 });

	auto x = full(
		descriptor, memory_resource_affinity::device, scalar_value(7.0f), context
	);
	auto y = full(
		descriptor, memory_resource_affinity::device, scalar_value(2.5f), context
	);
	const_array_ref x_ref = x;
	const_array_ref y_ref = y;

	const auto result = subtract(x_ref, y_ref, context, nullptr);

	CHECK( result.get_descriptor() == descriptor );
	for (const auto value : read_host<float>(result, 6))
	{
		CHECK( value == 4.5f );
	}
}

TEST_CASE_METHOD(
	cpu_execution_context_fixture,
	"subtract reuses the storage of the provided output array",
	"[array_arithmetic][cpu]"
)
{
	const auto descriptor = make_descriptor({ 2, 3 });

	auto x = full(
		descriptor, memory_resource_affinity::device, scalar_value(7.0f), context
	);
	auto y = full(
		descriptor, memory_resource_affinity::device, scalar_value(2.5f), context
	);
	const_array_ref x_ref = x;
	const_array_ref y_ref = y;

	auto out = zeros(descriptor, memory_resource_affinity::device, context);
	const auto *out_storage = out.get_storage();

	const auto result = subtract(x_ref, y_ref, context, &out);

	CHECK( result.get_storage() == out_storage );
	for (const auto value : read_host<float>(result, 6))
	{
		CHECK( value == 4.5f );
	}
}

TEST_CASE_METHOD(
	cpu_execution_context_fixture,
	"multiply computes the element-wise product of two arrays",
	"[array_arithmetic][cpu]"
)
{
	const auto descriptor = make_descriptor({ 2, 3 });

	auto x = full(
		descriptor, memory_resource_affinity::device, scalar_value(2.0f), context
	);
	auto y = full(
		descriptor, memory_resource_affinity::device, scalar_value(4.0f), context
	);
	const_array_ref x_ref = x;
	const_array_ref y_ref = y;

	const auto result = multiply(x_ref, y_ref, context, nullptr);

	CHECK( result.get_descriptor() == descriptor );
	for (const auto value : read_host<float>(result, 6))
	{
		CHECK( value == 8.0f );
	}
}

TEST_CASE_METHOD(
	cpu_execution_context_fixture,
	"multiply reuses the storage of the provided output array",
	"[array_arithmetic][cpu]"
)
{
	const auto descriptor = make_descriptor({ 2, 3 });

	auto x = full(
		descriptor, memory_resource_affinity::device, scalar_value(2.0f), context
	);
	auto y = full(
		descriptor, memory_resource_affinity::device, scalar_value(4.0f), context
	);
	const_array_ref x_ref = x;
	const_array_ref y_ref = y;

	auto out = zeros(descriptor, memory_resource_affinity::device, context);
	const auto *out_storage = out.get_storage();

	const auto result = multiply(x_ref, y_ref, context, &out);

	CHECK( result.get_storage() == out_storage );
	for (const auto value : read_host<float>(result, 6))
	{
		CHECK( value == 8.0f );
	}
}

TEST_CASE_METHOD(
	cpu_execution_context_fixture,
	"divide computes the element-wise quotient of two arrays",
	"[array_arithmetic][cpu]"
)
{
	const auto descriptor = make_descriptor({ 2, 3 });

	auto x = full(
		descriptor, memory_resource_affinity::device, scalar_value(9.0f), context
	);
	auto y = full(
		descriptor, memory_resource_affinity::device, scalar_value(2.0f), context
	);
	const_array_ref x_ref = x;
	const_array_ref y_ref = y;

	const auto result = divide(x_ref, y_ref, context, nullptr);

	CHECK( result.get_descriptor() == descriptor );
	for (const auto value : read_host<float>(result, 6))
	{
		CHECK( value == 4.5f );
	}
}

TEST_CASE_METHOD(
	cpu_execution_context_fixture,
	"divide reuses the storage of the provided output array",
	"[array_arithmetic][cpu]"
)
{
	const auto descriptor = make_descriptor({ 2, 3 });

	auto x = full(
		descriptor, memory_resource_affinity::device, scalar_value(9.0f), context
	);
	auto y = full(
		descriptor, memory_resource_affinity::device, scalar_value(2.0f), context
	);
	const_array_ref x_ref = x;
	const_array_ref y_ref = y;

	auto out = zeros(descriptor, memory_resource_affinity::device, context);
	const auto *out_storage = out.get_storage();

	const auto result = divide(x_ref, y_ref, context, &out);

	CHECK( result.get_storage() == out_storage );
	for (const auto value : read_host<float>(result, 6))
	{
		CHECK( value == 4.5f );
	}
}

TEST_CASE_METHOD(
	cpu_execution_context_fixture,
	"modulo computes the element-wise modulo of two arrays",
	"[array_arithmetic][cpu]"
)
{
	const auto descriptor = make_descriptor({ 2, 3 });

	auto x = full(
		descriptor, memory_resource_affinity::device, scalar_value(7.0f), context
	);
	auto y = full(
		descriptor, memory_resource_affinity::device, scalar_value(3.0f), context
	);
	const_array_ref x_ref = x;
	const_array_ref y_ref = y;

	const auto result = modulo(x_ref, y_ref, context, nullptr);

	CHECK( result.get_descriptor() == descriptor );
	for (const auto value : read_host<float>(result, 6))
	{
		CHECK( value == 1.0f );
	}
}

TEST_CASE_METHOD(
	cpu_execution_context_fixture,
	"modulo result takes the sign of the divisor for a negative dividend, "
	"unlike C's modulo",
	"[array_arithmetic][cpu]"
)
{
	const auto descriptor = make_descriptor({ 2, 3 });

	auto x = full(
		descriptor, memory_resource_affinity::device, scalar_value(-7.0f), context
	);
	auto y = full(
		descriptor, memory_resource_affinity::device, scalar_value(3.0f), context
	);
	const_array_ref x_ref = x;
	const_array_ref y_ref = y;

	const auto result = modulo(x_ref, y_ref, context, nullptr);

	CHECK( result.get_descriptor() == descriptor );
	for (const auto value : read_host<float>(result, 6))
	{
		CHECK( value == 2.0f );
	}
}

TEST_CASE_METHOD(
	cpu_execution_context_fixture,
	"modulo result takes the sign of the divisor for a negative divisor, "
	"unlike C's modulo",
	"[array_arithmetic][cpu]"
)
{
	const auto descriptor = make_descriptor({ 2, 3 });

	auto x = full(
		descriptor, memory_resource_affinity::device, scalar_value(7.0f), context
	);
	auto y = full(
		descriptor, memory_resource_affinity::device, scalar_value(-3.0f), context
	);
	const_array_ref x_ref = x;
	const_array_ref y_ref = y;

	const auto result = modulo(x_ref, y_ref, context, nullptr);

	CHECK( result.get_descriptor() == descriptor );
	for (const auto value : read_host<float>(result, 6))
	{
		CHECK( value == -2.0f );
	}
}

TEST_CASE_METHOD(
	cpu_execution_context_fixture,
	"modulo reuses the storage of the provided output array",
	"[array_arithmetic][cpu]"
)
{
	const auto descriptor = make_descriptor({ 2, 3 });

	auto x = full(
		descriptor, memory_resource_affinity::device, scalar_value(-7.0f), context
	);
	auto y = full(
		descriptor, memory_resource_affinity::device, scalar_value(3.0f), context
	);
	const_array_ref x_ref = x;
	const_array_ref y_ref = y;

	auto out = zeros(descriptor, memory_resource_affinity::device, context);
	const auto *out_storage = out.get_storage();

	const auto result = modulo(x_ref, y_ref, context, &out);

	CHECK( result.get_storage() == out_storage );
	for (const auto value : read_host<float>(result, 6))
	{
		CHECK( value == 2.0f );
	}
}

TEST_CASE_METHOD(
	cpu_execution_context_fixture,
	"negate computes the element-wise negation of an array",
	"[array_arithmetic][cpu]"
)
{
	const auto descriptor = make_descriptor({ 2, 3 });

	auto x = full(
		descriptor, memory_resource_affinity::device, scalar_value(3.0f), context
	);
	const_array_ref x_ref = x;

	const auto result = negate(x_ref, context, nullptr);

	CHECK( result.get_descriptor() == descriptor );
	for (const auto value : read_host<float>(result, 6))
	{
		CHECK( value == -3.0f );
	}
}

TEST_CASE_METHOD(
	cpu_execution_context_fixture,
	"negate reuses the storage of the provided output array",
	"[array_arithmetic][cpu]"
)
{
	const auto descriptor = make_descriptor({ 2, 3 });

	auto x = full(
		descriptor, memory_resource_affinity::device, scalar_value(3.0f), context
	);
	const_array_ref x_ref = x;

	auto out = zeros(descriptor, memory_resource_affinity::device, context);
	const auto *out_storage = out.get_storage();

	const auto result = negate(x_ref, context, &out);

	CHECK( result.get_storage() == out_storage );
	for (const auto value : read_host<float>(result, 6))
	{
		CHECK( value == -3.0f );
	}
}

TEST_CASE_METHOD(
	cpu_execution_context_fixture,
	"abs computes the element-wise absolute value of an array",
	"[array_arithmetic][cpu]"
)
{
	const auto descriptor = make_descriptor({ 2, 3 });

	auto x = full(
		descriptor, memory_resource_affinity::device, scalar_value(-3.0f), context
	);
	const_array_ref x_ref = x;

	const auto result = abs(x_ref, context, nullptr);

	CHECK( result.get_descriptor() == descriptor );
	for (const auto value : read_host<float>(result, 6))
	{
		CHECK( value == 3.0f );
	}
}

TEST_CASE_METHOD(
	cpu_execution_context_fixture,
	"abs reuses the storage of the provided output array",
	"[array_arithmetic][cpu]"
)
{
	const auto descriptor = make_descriptor({ 2, 3 });

	auto x = full(
		descriptor, memory_resource_affinity::device, scalar_value(-3.0f), context
	);
	const_array_ref x_ref = x;

	auto out = zeros(descriptor, memory_resource_affinity::device, context);
	const auto *out_storage = out.get_storage();

	const auto result = abs(x_ref, context, &out);

	CHECK( result.get_storage() == out_storage );
	for (const auto value : read_host<float>(result, 6))
	{
		CHECK( value == 3.0f );
	}
}
