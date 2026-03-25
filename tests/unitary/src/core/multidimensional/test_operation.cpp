// SPDX-License-Identifier: GPL-3.0-only

#include <catch2/catch_test_macros.hpp>

#include <xmipp4/core/multidimensional/operation.hpp>

#include "mock/mock_operation.hpp"

using namespace xmipp4;
using namespace xmipp4::multidimensional;

TEST_CASE("Operations should return the correct operation_id", "[operation]")
{
	mock_operation op;
	CHECK( op.get_id() == operation_id::of<mock_operation>() );
}

TEST_CASE("Operations should return an empty parameter serialization by default", "[operation]")
{
	mock_operation op;

	REQUIRE_CALL(op, serialize_parameters())
		.LR_RETURN(op.operation::serialize_parameters());

	CHECK( op.serialize_parameters() == "" );
}
