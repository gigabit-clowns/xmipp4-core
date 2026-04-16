// SPDX-License-Identifier: GPL-3.0-only

#include <catch2/catch_test_macros.hpp>

#include <xmipp4/core/multidimensional/operations/copy_operation.hpp>
#include <xmipp4/core/multidimensional/shape_policies/elementwise_shape_policy.hpp>

#include <core/multidimensional/data_type_policies/copy_data_type_policy.hpp>

using namespace xmipp4;
using namespace xmipp4::multidimensional;

TEST_CASE("get_name in copy_operation should return copy", "[copy_operation]")
{
	copy_operation op;
	CHECK( op.get_name() == "copy" );
}

TEST_CASE("get_shape_policy in copy_operation should return the elementwise_shape_policy singleton", "[copy_operation]")
{
	copy_operation op;
	CHECK( &op.get_shape_policy() == &elementwise_shape_policy::get() );
}

TEST_CASE("get_data_type_policy in copy_operation should return the copy_operation_data_type_policy singleton", "[copy_operation]")
{
	copy_operation op;
	CHECK( &op.get_data_type_policy() == &copy_data_type_policy::get() );
}
