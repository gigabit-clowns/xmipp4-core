// SPDX-License-Identifier: GPL-3.0-only

#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_template_test_macros.hpp>

#include <xmipp4/functional/arithmetic.hpp>

#include "fixtures/cpu_execution_context_fixture.hpp"

#include <xmipp4/functional/creation.hpp>
#include <xmipp4/core/meta/type_list.hpp>
#include <xmipp4/core/ndarray/array.hpp>
#include <xmipp4/core/ndarray/const_array.hpp>
#include <xmipp4/core/ndarray/const_array_ref.hpp>
#include <xmipp4/core/hardware/memory_resource_affinity.hpp>
#include <xmipp4/core/numerical/numerical_type_traits.hpp>
#include <xmipp4/core/numerical/scalar_value.hpp>

#include <complex>
#include <cstdint>
#include <stdexcept>

using namespace xmipp4;

namespace
{

// TEMPLATE_LIST_TEST_CASE_METHOD requires the fixture to be a class template.
template <typename T>
class cpu_execution_context_fixture_tmpl : public cpu_execution_context_fixture
{
};

using signed_int_types = type_list<
	std::int8_t, std::int16_t, std::int32_t, std::int64_t
>;
using unsigned_int_types = type_list<
	std::uint8_t, std::uint16_t, std::uint32_t, std::uint64_t
>;
using float_types = type_list<float16_t, float32_t, float64_t>;
using complex_types = type_list<
	std::complex<float16_t>, std::complex<float32_t>, std::complex<float64_t>
>;
using char_types = type_list<char>;
using bool_types = type_list<bool>;

using add_types = type_list_cat_t<
	signed_int_types, unsigned_int_types, char_types, float_types
>;
using subtract_types = type_list_cat_t<
	signed_int_types, unsigned_int_types, char_types, float_types
>;
using multiply_types = type_list_cat_t<
	signed_int_types, unsigned_int_types, char_types, float_types
>;
using divide_types = type_list_cat_t<
	signed_int_types, unsigned_int_types, char_types, float_types
>;
using modulo_types = type_list_cat_t<
	signed_int_types, unsigned_int_types, char_types, float_types
>;
using modulo_sign_types = type_list_cat_t<signed_int_types, float_types>;
using negate_types = type_list_cat_t<signed_int_types, float_types>;
using abs_signed_types = type_list_cat_t<signed_int_types, float_types>;

using negate_rejected_types = type_list_cat_t<
	bool_types, unsigned_int_types, complex_types
>;
using modulo_rejected_types = type_list_cat_t<bool_types, complex_types>;
using abs_rejected_types = type_list_cat_t<bool_types, char_types>;

} // namespace


TEMPLATE_LIST_TEST_CASE_METHOD(
	cpu_execution_context_fixture_tmpl,
	"add computes the element-wise sum of two arrays",
	"[array_arithmetic][cpu]",
	add_types
)
{
	const auto descriptor =
		this->make_descriptor({ 2, 3 }, numerical_type_of<TestType>::value);

	auto x = full(
		descriptor, memory_resource_affinity::device,
		scalar_value(static_cast<TestType>(2)), this->context
	);
	auto y = full(
		descriptor, memory_resource_affinity::device,
		scalar_value(static_cast<TestType>(3)), this->context
	);
	const_array_ref x_ref = x;
	const_array_ref y_ref = y;

	const auto result = add(x_ref, y_ref, this->context, nullptr);

	CHECK( result.get_descriptor() == descriptor );
	for (const auto value : this->template read_host<TestType>(result, 6))
	{
		CHECK( value == static_cast<TestType>(5) );
	}
}

TEMPLATE_LIST_TEST_CASE_METHOD(
	cpu_execution_context_fixture_tmpl,
	"add computes the element-wise sum of two complex arrays",
	"[array_arithmetic][cpu]",
	complex_types
)
{
	using R = typename TestType::value_type;
	const auto descriptor =
		this->make_descriptor({ 2, 3 }, numerical_type_of<TestType>::value);

	auto x = full(
		descriptor, memory_resource_affinity::device,
		scalar_value(TestType(2, 1)), this->context
	);
	auto y = full(
		descriptor, memory_resource_affinity::device,
		scalar_value(TestType(3, 2)), this->context
	);
	const_array_ref x_ref = x;
	const_array_ref y_ref = y;

	const auto result = add(x_ref, y_ref, this->context, nullptr);

	CHECK( result.get_descriptor() == descriptor );
	for (const auto value : this->template read_host<TestType>(result, 6))
	{
		CHECK( static_cast<R>(value.real()) == static_cast<R>(5) );
		CHECK( static_cast<R>(value.imag()) == static_cast<R>(3) );
	}
}

TEST_CASE_METHOD(
	cpu_execution_context_fixture,
	"add computes the element-wise sum of two boolean arrays",
	"[array_arithmetic][cpu]"
)
{
	const auto descriptor = make_descriptor({ 2, 3 }, numerical_type::boolean);

	// bool is kept out of add_types: for booleans, sum/difference/product are
	// implemented with ||/!=/&& instead of +/-/* to avoid a Windows compiler
	// warning, but the result is identical elementwise, so it still tests the
	// sum, just via a dedicated boolean-typed array.
	auto x = full(
		descriptor, memory_resource_affinity::device, scalar_value(false), context
	);
	auto y = full(
		descriptor, memory_resource_affinity::device, scalar_value(true), context
	);
	const_array_ref x_ref = x;
	const_array_ref y_ref = y;

	const auto result = add(x_ref, y_ref, context, nullptr);

	CHECK( result.get_descriptor() == descriptor );
	for (const auto value : read_host<bool>(result, 6))
	{
		CHECK( value == true );
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

TEMPLATE_LIST_TEST_CASE_METHOD(
	cpu_execution_context_fixture_tmpl,
	"subtract computes the element-wise difference of two arrays",
	"[array_arithmetic][cpu]",
	subtract_types
)
{
	const auto descriptor =
		this->make_descriptor({ 2, 3 }, numerical_type_of<TestType>::value);

	auto x = full(
		descriptor, memory_resource_affinity::device,
		scalar_value(static_cast<TestType>(7)), this->context
	);
	auto y = full(
		descriptor, memory_resource_affinity::device,
		scalar_value(static_cast<TestType>(2)), this->context
	);
	const_array_ref x_ref = x;
	const_array_ref y_ref = y;

	const auto result = subtract(x_ref, y_ref, this->context, nullptr);

	CHECK( result.get_descriptor() == descriptor );
	for (const auto value : this->template read_host<TestType>(result, 6))
	{
		CHECK( value == static_cast<TestType>(5) );
	}
}

TEMPLATE_LIST_TEST_CASE_METHOD(
	cpu_execution_context_fixture_tmpl,
	"subtract computes the element-wise difference of two complex arrays",
	"[array_arithmetic][cpu]",
	complex_types
)
{
	using R = typename TestType::value_type;
	const auto descriptor =
		this->make_descriptor({ 2, 3 }, numerical_type_of<TestType>::value);

	auto x = full(
		descriptor, memory_resource_affinity::device,
		scalar_value(TestType(7, 4)), this->context
	);
	auto y = full(
		descriptor, memory_resource_affinity::device,
		scalar_value(TestType(2, 1)), this->context
	);
	const_array_ref x_ref = x;
	const_array_ref y_ref = y;

	const auto result = subtract(x_ref, y_ref, this->context, nullptr);

	CHECK( result.get_descriptor() == descriptor );
	for (const auto value : this->template read_host<TestType>(result, 6))
	{
		CHECK( static_cast<R>(value.real()) == static_cast<R>(5) );
		CHECK( static_cast<R>(value.imag()) == static_cast<R>(3) );
	}
}

TEST_CASE_METHOD(
	cpu_execution_context_fixture,
	"subtract computes the element-wise difference of two boolean arrays",
	"[array_arithmetic][cpu]"
)
{
	const auto descriptor = make_descriptor({ 2, 3 }, numerical_type::boolean);

	auto x = full(
		descriptor, memory_resource_affinity::device, scalar_value(true), context
	);
	auto y = full(
		descriptor, memory_resource_affinity::device, scalar_value(true), context
	);
	const_array_ref x_ref = x;
	const_array_ref y_ref = y;

	const auto result = subtract(x_ref, y_ref, context, nullptr);

	CHECK( result.get_descriptor() == descriptor );
	for (const auto value : read_host<bool>(result, 6))
	{
		CHECK( value == false );
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

TEMPLATE_LIST_TEST_CASE_METHOD(
	cpu_execution_context_fixture_tmpl,
	"multiply computes the element-wise product of two arrays",
	"[array_arithmetic][cpu]",
	multiply_types
)
{
	const auto descriptor =
		this->make_descriptor({ 2, 3 }, numerical_type_of<TestType>::value);

	auto x = full(
		descriptor, memory_resource_affinity::device,
		scalar_value(static_cast<TestType>(2)), this->context
	);
	auto y = full(
		descriptor, memory_resource_affinity::device,
		scalar_value(static_cast<TestType>(4)), this->context
	);
	const_array_ref x_ref = x;
	const_array_ref y_ref = y;

	const auto result = multiply(x_ref, y_ref, this->context, nullptr);

	CHECK( result.get_descriptor() == descriptor );
	for (const auto value : this->template read_host<TestType>(result, 6))
	{
		CHECK( value == static_cast<TestType>(8) );
	}
}

TEMPLATE_LIST_TEST_CASE_METHOD(
	cpu_execution_context_fixture_tmpl,
	"multiply computes the element-wise product of two complex arrays",
	"[array_arithmetic][cpu]",
	complex_types
)
{
	using R = typename TestType::value_type;
	const auto descriptor =
		this->make_descriptor({ 2, 3 }, numerical_type_of<TestType>::value);

	auto x = full(
		descriptor, memory_resource_affinity::device,
		scalar_value(TestType(2, 3)), this->context
	);
	auto y = full(
		descriptor, memory_resource_affinity::device,
		scalar_value(TestType(1, 2)), this->context
	);
	const_array_ref x_ref = x;
	const_array_ref y_ref = y;

	const auto result = multiply(x_ref, y_ref, this->context, nullptr);

	CHECK( result.get_descriptor() == descriptor );
	// (2+3i)*(1+2i) = -4+7i
	for (const auto value : this->template read_host<TestType>(result, 6))
	{
		CHECK( static_cast<R>(value.real()) == static_cast<R>(-4) );
		CHECK( static_cast<R>(value.imag()) == static_cast<R>(7) );
	}
}

TEST_CASE_METHOD(
	cpu_execution_context_fixture,
	"multiply computes the element-wise product of two boolean arrays",
	"[array_arithmetic][cpu]"
)
{
	const auto descriptor = make_descriptor({ 2, 3 }, numerical_type::boolean);

	auto x = full(
		descriptor, memory_resource_affinity::device, scalar_value(true), context
	);
	auto y = full(
		descriptor, memory_resource_affinity::device, scalar_value(false), context
	);
	const_array_ref x_ref = x;
	const_array_ref y_ref = y;

	const auto result = multiply(x_ref, y_ref, context, nullptr);

	CHECK( result.get_descriptor() == descriptor );
	for (const auto value : read_host<bool>(result, 6))
	{
		CHECK( value == false );
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

TEMPLATE_LIST_TEST_CASE_METHOD(
	cpu_execution_context_fixture_tmpl,
	"divide computes the element-wise quotient of two arrays",
	"[array_arithmetic][cpu]",
	divide_types
)
{
	// Operands chosen so the quotient is exact for every supported type,
	// including integer types (which truncate). The float-specific
	// fractional-result behavior is checked separately below.
	const auto descriptor =
		this->make_descriptor({ 2, 3 }, numerical_type_of<TestType>::value);

	auto x = full(
		descriptor, memory_resource_affinity::device,
		scalar_value(static_cast<TestType>(8)), this->context
	);
	auto y = full(
		descriptor, memory_resource_affinity::device,
		scalar_value(static_cast<TestType>(2)), this->context
	);
	const_array_ref x_ref = x;
	const_array_ref y_ref = y;

	const auto result = divide(x_ref, y_ref, this->context, nullptr);

	CHECK( result.get_descriptor() == descriptor );
	for (const auto value : this->template read_host<TestType>(result, 6))
	{
		CHECK( value == static_cast<TestType>(4) );
	}
}

TEMPLATE_LIST_TEST_CASE_METHOD(
	cpu_execution_context_fixture_tmpl,
	"divide gives a fractional result for floating point arrays, "
	"unlike integer division",
	"[array_arithmetic][cpu]",
	float_types
)
{
	const auto descriptor =
		this->make_descriptor({ 2, 3 }, numerical_type_of<TestType>::value);

	auto x = full(
		descriptor, memory_resource_affinity::device,
		scalar_value(static_cast<TestType>(9)), this->context
	);
	auto y = full(
		descriptor, memory_resource_affinity::device,
		scalar_value(static_cast<TestType>(2)), this->context
	);
	const_array_ref x_ref = x;
	const_array_ref y_ref = y;

	const auto result = divide(x_ref, y_ref, this->context, nullptr);

	CHECK( result.get_descriptor() == descriptor );
	for (const auto value : this->template read_host<TestType>(result, 6))
	{
		CHECK( value == static_cast<TestType>(4.5) );
	}
}

TEMPLATE_LIST_TEST_CASE_METHOD(
	cpu_execution_context_fixture_tmpl,
	"divide computes the element-wise quotient of two complex arrays",
	"[array_arithmetic][cpu]",
	complex_types
)
{
	using R = typename TestType::value_type;
	const auto descriptor =
		this->make_descriptor({ 2, 3 }, numerical_type_of<TestType>::value);

	// Dividing by a real-valued complex number keeps the expected value
	// simple: (a+bi) / (c+0i) = (a/c) + (b/c)i.
	auto x = full(
		descriptor, memory_resource_affinity::device,
		scalar_value(TestType(8, 4)), this->context
	);
	auto y = full(
		descriptor, memory_resource_affinity::device,
		scalar_value(TestType(2, 0)), this->context
	);
	const_array_ref x_ref = x;
	const_array_ref y_ref = y;

	const auto result = divide(x_ref, y_ref, this->context, nullptr);

	CHECK( result.get_descriptor() == descriptor );
	for (const auto value : this->template read_host<TestType>(result, 6))
	{
		CHECK( static_cast<R>(value.real()) == static_cast<R>(4) );
		CHECK( static_cast<R>(value.imag()) == static_cast<R>(2) );
	}
}

TEST_CASE_METHOD(
	cpu_execution_context_fixture,
	"divide rejects boolean arrays",
	"[array_arithmetic][cpu]"
)
{
	const auto descriptor = make_descriptor({ 2, 3 }, numerical_type::boolean);

	auto x = full(
		descriptor, memory_resource_affinity::device, scalar_value(true), context
	);
	auto y = full(
		descriptor, memory_resource_affinity::device, scalar_value(true), context
	);
	const_array_ref x_ref = x;
	const_array_ref y_ref = y;

	CHECK_THROWS_AS( divide(x_ref, y_ref, context, nullptr), std::invalid_argument );
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

TEMPLATE_LIST_TEST_CASE_METHOD(
	cpu_execution_context_fixture_tmpl,
	"modulo computes the element-wise modulo of two arrays",
	"[array_arithmetic][cpu]",
	modulo_types
)
{
	const auto descriptor =
		this->make_descriptor({ 2, 3 }, numerical_type_of<TestType>::value);

	auto x = full(
		descriptor, memory_resource_affinity::device,
		scalar_value(static_cast<TestType>(7)), this->context
	);
	auto y = full(
		descriptor, memory_resource_affinity::device,
		scalar_value(static_cast<TestType>(3)), this->context
	);
	const_array_ref x_ref = x;
	const_array_ref y_ref = y;

	const auto result = modulo(x_ref, y_ref, this->context, nullptr);

	CHECK( result.get_descriptor() == descriptor );
	for (const auto value : this->template read_host<TestType>(result, 6))
	{
		CHECK( value == static_cast<TestType>(1) );
	}
}

TEMPLATE_LIST_TEST_CASE_METHOD(
	cpu_execution_context_fixture_tmpl,
	"modulo result takes the sign of the divisor for a negative dividend, "
	"unlike C's modulo",
	"[array_arithmetic][cpu]",
	modulo_sign_types
)
{
	const auto descriptor =
		this->make_descriptor({ 2, 3 }, numerical_type_of<TestType>::value);

	auto x = full(
		descriptor, memory_resource_affinity::device,
		scalar_value(static_cast<TestType>(-7)), this->context
	);
	auto y = full(
		descriptor, memory_resource_affinity::device,
		scalar_value(static_cast<TestType>(3)), this->context
	);
	const_array_ref x_ref = x;
	const_array_ref y_ref = y;

	const auto result = modulo(x_ref, y_ref, this->context, nullptr);

	CHECK( result.get_descriptor() == descriptor );
	for (const auto value : this->template read_host<TestType>(result, 6))
	{
		CHECK( value == static_cast<TestType>(2) );
	}
}

TEMPLATE_LIST_TEST_CASE_METHOD(
	cpu_execution_context_fixture_tmpl,
	"modulo result takes the sign of the divisor for a negative divisor, "
	"unlike C's modulo",
	"[array_arithmetic][cpu]",
	modulo_sign_types
)
{
	const auto descriptor =
		this->make_descriptor({ 2, 3 }, numerical_type_of<TestType>::value);

	auto x = full(
		descriptor, memory_resource_affinity::device,
		scalar_value(static_cast<TestType>(7)), this->context
	);
	auto y = full(
		descriptor, memory_resource_affinity::device,
		scalar_value(static_cast<TestType>(-3)), this->context
	);
	const_array_ref x_ref = x;
	const_array_ref y_ref = y;

	const auto result = modulo(x_ref, y_ref, this->context, nullptr);

	CHECK( result.get_descriptor() == descriptor );
	for (const auto value : this->template read_host<TestType>(result, 6))
	{
		CHECK( value == static_cast<TestType>(-2) );
	}
}

TEMPLATE_LIST_TEST_CASE_METHOD(
	cpu_execution_context_fixture_tmpl,
	"modulo rejects unsupported types",
	"[array_arithmetic][cpu]",
	modulo_rejected_types
)
{
	const auto descriptor =
		this->make_descriptor({ 2, 3 }, numerical_type_of<TestType>::value);

	auto x = full(
		descriptor, memory_resource_affinity::device,
		scalar_value(static_cast<TestType>(1)), this->context
	);
	auto y = full(
		descriptor, memory_resource_affinity::device,
		scalar_value(static_cast<TestType>(1)), this->context
	);
	const_array_ref x_ref = x;
	const_array_ref y_ref = y;

	CHECK_THROWS_AS( modulo(x_ref, y_ref, this->context, nullptr), std::invalid_argument );
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

TEMPLATE_LIST_TEST_CASE_METHOD(
	cpu_execution_context_fixture_tmpl,
	"negate computes the element-wise negation of an array",
	"[array_arithmetic][cpu]",
	negate_types
)
{
	const auto descriptor =
		this->make_descriptor({ 2, 3 }, numerical_type_of<TestType>::value);

	auto x = full(
		descriptor, memory_resource_affinity::device,
		scalar_value(static_cast<TestType>(3)), this->context
	);
	const_array_ref x_ref = x;

	const auto result = negate(x_ref, this->context, nullptr);

	CHECK( result.get_descriptor() == descriptor );
	for (const auto value : this->template read_host<TestType>(result, 6))
	{
		CHECK( value == static_cast<TestType>(-3) );
	}
}

TEMPLATE_LIST_TEST_CASE_METHOD(
	cpu_execution_context_fixture_tmpl,
	"negate rejects unsupported types",
	"[array_arithmetic][cpu]",
	negate_rejected_types
)
{
	const auto descriptor =
		this->make_descriptor({ 2, 3 }, numerical_type_of<TestType>::value);

	auto x = full(
		descriptor, memory_resource_affinity::device,
		scalar_value(static_cast<TestType>(1)), this->context
	);
	const_array_ref x_ref = x;

	CHECK_THROWS_AS( negate(x_ref, this->context, nullptr), std::invalid_argument );
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

TEMPLATE_LIST_TEST_CASE_METHOD(
	cpu_execution_context_fixture_tmpl,
	"abs computes the element-wise absolute value of a signed array",
	"[array_arithmetic][cpu]",
	abs_signed_types
)
{
	const auto descriptor =
		this->make_descriptor({ 2, 3 }, numerical_type_of<TestType>::value);

	auto x = full(
		descriptor, memory_resource_affinity::device,
		scalar_value(static_cast<TestType>(-3)), this->context
	);
	const_array_ref x_ref = x;

	const auto result = abs(x_ref, this->context, nullptr);

	CHECK( result.get_descriptor() == descriptor );
	for (const auto value : this->template read_host<TestType>(result, 6))
	{
		CHECK( value == static_cast<TestType>(3) );
	}
}

TEMPLATE_LIST_TEST_CASE_METHOD(
	cpu_execution_context_fixture_tmpl,
	"abs is the identity for an unsigned array",
	"[array_arithmetic][cpu]",
	unsigned_int_types
)
{
	const auto descriptor =
		this->make_descriptor({ 2, 3 }, numerical_type_of<TestType>::value);

	auto x = full(
		descriptor, memory_resource_affinity::device,
		scalar_value(static_cast<TestType>(3)), this->context
	);
	const_array_ref x_ref = x;

	const auto result = abs(x_ref, this->context, nullptr);

	CHECK( result.get_descriptor() == descriptor );
	for (const auto value : this->template read_host<TestType>(result, 6))
	{
		CHECK( value == static_cast<TestType>(3) );
	}
}

TEMPLATE_LIST_TEST_CASE_METHOD(
	cpu_execution_context_fixture_tmpl,
	"abs computes the element-wise magnitude of a complex array, "
	"producing the real equivalent type",
	"[array_arithmetic][cpu]",
	complex_types
)
{
	using R = typename TestType::value_type;
	const auto in_type = numerical_type_of<TestType>::value;
	const auto out_type = make_real(in_type);
	const auto in_descriptor = this->make_descriptor({ 2, 3 }, in_type);
	const auto out_descriptor = this->make_descriptor({ 2, 3 }, out_type);

	// 3-4-5 triangle: magnitude is exact even after a floating point sqrt.
	auto x = full(
		in_descriptor, memory_resource_affinity::device,
		scalar_value(TestType(3, 4)), this->context
	);
	const_array_ref x_ref = x;

	const auto result = abs(x_ref, this->context, nullptr);

	CHECK( result.get_descriptor() == out_descriptor );
	for (const auto value : this->template read_host<R>(result, 6))
	{
		CHECK( value == static_cast<R>(5) );
	}
}

TEMPLATE_LIST_TEST_CASE_METHOD(
	cpu_execution_context_fixture_tmpl,
	"abs rejects unsupported types",
	"[array_arithmetic][cpu]",
	abs_rejected_types
)
{
	const auto descriptor =
		this->make_descriptor({ 2, 3 }, numerical_type_of<TestType>::value);

	auto x = full(
		descriptor, memory_resource_affinity::device,
		scalar_value(static_cast<TestType>(1)), this->context
	);
	const_array_ref x_ref = x;

	CHECK_THROWS_AS( abs(x_ref, this->context, nullptr), std::invalid_argument );
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
