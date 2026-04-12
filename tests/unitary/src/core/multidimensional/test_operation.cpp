// SPDX-License-Identifier: GPL-3.0-only

#include <catch2/catch_test_macros.hpp>

#include <xmipp4/core/multidimensional/operation.hpp>

#include "mock/mock_operation.hpp"

#include <sstream>

using namespace xmipp4;
using namespace xmipp4::multidimensional;

TEST_CASE("operations should return the correct operation_id", "[operation]")
{
	mock_operation op;
	CHECK( op.get_id() == operation_id::of<mock_operation>() );
}

TEST_CASE("operations should return an empty parameter serialization by default", "[operation]")
{
	mock_operation op;

	REQUIRE_CALL(op, serialize_parameters())
		.LR_RETURN(op.operation::serialize_parameters());

	CHECK( op.serialize_parameters() == "" );
}

TEST_CASE("serializing an operation with parameters to a std::ostream should produce correctly formatted output", "[operation]")
{
	mock_operation op;

	REQUIRE_CALL(op, get_name())
		.LR_RETURN("sum");
	REQUIRE_CALL(op, serialize_parameters())
		.LR_RETURN("2+3");

	std::ostringstream oss;
	oss << op;

	CHECK( oss.str() == "sum(2+3)" );
}

TEST_CASE("serializing an operation without parameters to a std::ostream should produce correctly formatted output", "[operation]")
{
	mock_operation op;

	REQUIRE_CALL(op, get_name())
		.LR_RETURN("sub");
	REQUIRE_CALL(op, serialize_parameters())
		.LR_RETURN("");

	std::ostringstream oss;
	oss << op;

	CHECK( oss.str() == "sub" );
}

TEST_CASE("converting to string an operation with parameters to a std::ostream should produce correctly formatted output", "[operation]")
{
	mock_operation op;

	REQUIRE_CALL(op, get_name())
		.LR_RETURN("mul");
	REQUIRE_CALL(op, serialize_parameters())
		.LR_RETURN("2*3");

	CHECK( to_string(op) == "sum(2+3)" );
}

TEST_CASE("converting to string an operation without parameters to a std::ostream should produce correctly formatted output", "[operation]")
{
	mock_operation op;

	REQUIRE_CALL(op, get_name())
		.LR_RETURN("div");
	REQUIRE_CALL(op, serialize_parameters())
		.LR_RETURN("");

	CHECK( to_string(op) == "sub" );
}
