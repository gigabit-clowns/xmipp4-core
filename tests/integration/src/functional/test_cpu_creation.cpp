// SPDX-License-Identifier: GPL-3.0-only

#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_template_test_macros.hpp>

#include <xmipp4/functional/creation.hpp>

#include "fixtures/cpu_execution_context_fixture.hpp"

#include <xmipp4/core/meta/type_list.hpp>
#include <xmipp4/core/ndarray/array.hpp>
#include <xmipp4/core/ndarray/const_array.hpp>
#include <xmipp4/core/hardware/memory_resource_affinity.hpp>
#include <xmipp4/core/numerical/numerical_type_traits.hpp>
#include <xmipp4/core/numerical/scalar_value.hpp>

#include <complex>
#include <cstdint>

using namespace xmipp4;

namespace
{

// TEMPLATE_LIST_TEST_CASE_METHOD requires the fixture to be a class template.
template <typename T>
class cpu_execution_context_fixture_tmpl : public cpu_execution_context_fixture
{
};

using all_types = type_list_cat_t<
	type_list<bool>,
	type_list<char>,
	type_list<std::int8_t, std::int16_t, std::int32_t, std::int64_t>,
	type_list<std::uint8_t, std::uint16_t, std::uint32_t, std::uint64_t>,
	type_list<float16_t, float32_t, float64_t>,
	type_list<
		std::complex<float16_t>, std::complex<float32_t>, std::complex<float64_t>
	>
>;

} // namespace


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

TEMPLATE_LIST_TEST_CASE_METHOD(
	cpu_execution_context_fixture_tmpl,
	"zeros sets every element of a host array to zero",
	"[array_creation][cpu]",
	all_types
)
{
	const auto descriptor =
		this->make_descriptor({ 2, 3 }, numerical_type_of<TestType>::value);

	const auto result = zeros(
		descriptor,
		memory_resource_affinity::host,
		this->context
	);

	CHECK( result.get_descriptor() == descriptor );

	for (const auto value : this->template read_host<TestType>(result, 6))
	{
		CHECK( value == static_cast<TestType>(0) );
	}
}

TEMPLATE_LIST_TEST_CASE_METHOD(
	cpu_execution_context_fixture_tmpl,
	"ones sets every element of a host array to one",
	"[array_creation][cpu]",
	all_types
)
{
	const auto descriptor =
		this->make_descriptor({ 2, 3 }, numerical_type_of<TestType>::value);

	const auto result = ones(
		descriptor,
		memory_resource_affinity::host,
		this->context
	);

	CHECK( result.get_descriptor() == descriptor );

	for (const auto value : this->template read_host<TestType>(result, 6))
	{
		CHECK( value == static_cast<TestType>(1) );
	}
}

TEMPLATE_LIST_TEST_CASE_METHOD(
	cpu_execution_context_fixture_tmpl,
	"full sets every element of a host array to the requested value",
	"[array_creation][cpu]",
	all_types
)
{
	const auto descriptor =
		this->make_descriptor({ 4 }, numerical_type_of<TestType>::value);

	const auto result = full(
		descriptor,
		memory_resource_affinity::host,
		scalar_value(static_cast<TestType>(3)),
		this->context
	);

	for (const auto value : this->template read_host<TestType>(result, 4))
	{
		CHECK( value == static_cast<TestType>(3) );
	}
}

TEMPLATE_LIST_TEST_CASE_METHOD(
	cpu_execution_context_fixture_tmpl,
	"fill overwrites the contents of an existing host array",
	"[array_creation][cpu]",
	all_types
)
{
	const auto descriptor =
		this->make_descriptor({ 5 }, numerical_type_of<TestType>::value);

	// Start from zeros so a successful fill is unambiguous.
	auto result = zeros(
		descriptor,
		memory_resource_affinity::host,
		this->context
	);

	fill(result, scalar_value(static_cast<TestType>(2)), this->context);

	for (const auto value : this->template read_host<TestType>(result, 5))
	{
		CHECK( value == static_cast<TestType>(2) );
	}
}

TEMPLATE_LIST_TEST_CASE_METHOD(
	cpu_execution_context_fixture_tmpl,
	"copy duplicates the source contents into independent storage",
	"[array_creation][cpu]",
	all_types
)
{
	const auto descriptor =
		this->make_descriptor({ 2, 3 }, numerical_type_of<TestType>::value);

	const auto source = full(
		descriptor,
		memory_resource_affinity::host,
		scalar_value(static_cast<TestType>(4)),
		this->context
	);

	const auto result = copy(source, this->context);

	CHECK( result.get_descriptor() == descriptor );

	// The copy owns a distinct buffer from the source.
	CHECK( result.get_storage() != source.get_storage() );

	for (const auto value : this->template read_host<TestType>(result, 6))
	{
		CHECK( value == static_cast<TestType>(4) );
	}
}
