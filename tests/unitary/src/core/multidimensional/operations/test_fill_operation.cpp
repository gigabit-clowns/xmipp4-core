// SPDX-License-Identifier: GPL-3.0-only

#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>

#include <xmipp4/core/multidimensional/operations/fill_operation.hpp>
#include <xmipp4/core/multidimensional/operations/policies/elementwise_shape_policy.hpp>
#include <xmipp4/core/multidimensional/operations/policies/same_data_type_policy.hpp>

using namespace xmipp4;
using namespace xmipp4::multidimensional;

TEST_CASE("get_name in fill_operation should return fill", "[fill_operation]")
{
	std::int8_t x = 2;
	fill_operation op(x);
	CHECK( op.get_name() == "fill" );
}

TEST_CASE("serialize_parameters should return the same representation for equal fill values", "[fill_operation]")
{
	std::int16_t value11 = 1;
	std::int16_t value12 = 1;
	float64_t value21 = 3.4;
	float64_t value22 = 3.4;
	std::complex<float32_t> value31(2.0f, 24.0f);
	std::complex<float32_t> value32(2.0f, 24.0f);

	CHECK( fill_operation(value11).serialize_parameters() == fill_operation(value12).serialize_parameters() );
	CHECK( fill_operation(value21).serialize_parameters() == fill_operation(value22).serialize_parameters() );
	CHECK( fill_operation(value31).serialize_parameters() == fill_operation(value32).serialize_parameters() );
}

TEST_CASE("serialize_parameters should return the unequal representation for unequal fill values", "[fill_operation]")
{
	std::int16_t value11 = 1;
	std::int16_t value12 = 2;
	float64_t value21 = 3.4;
	float64_t value22 = 3.3;
	std::complex<float32_t> value31(2.0f, 23.0f);
	std::complex<float32_t> value32(2.0f, 24.0f);

	CHECK( fill_operation(value11).serialize_parameters() != fill_operation(value12).serialize_parameters() );
	CHECK( fill_operation(value21).serialize_parameters() != fill_operation(value22).serialize_parameters() );
	CHECK( fill_operation(value31).serialize_parameters() != fill_operation(value32).serialize_parameters() );
}

TEST_CASE("get_shape_policy in fill_operation should return the elementwise_shape_policy singleton", "[fill_operation]")
{
	std::int8_t x = 0;
	fill_operation op(x);
	CHECK( &op.get_shape_policy() == &elementwise_shape_policy::get() );
}

TEST_CASE("get_data_type_policy in fill_operation should return the same_data_type_policy singleton", "[fill_operation]")
{
	std::int8_t x = 0;
	fill_operation op(x);
	CHECK( &op.get_data_type_policy() == &same_data_type_policy::get() );
}
