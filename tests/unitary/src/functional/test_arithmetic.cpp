// SPDX-License-Identifier: GPL-3.0-only

#include <catch2/catch_test_macros.hpp>

#include <xmipp4/functional/arithmetic.hpp>

#include <xmipp4/ops/arithmetic/abs_operation.hpp>
#include <xmipp4/ops/arithmetic/add_operation.hpp>
#include <xmipp4/ops/arithmetic/divide_operation.hpp>
#include <xmipp4/ops/arithmetic/modulo_operation.hpp>
#include <xmipp4/ops/arithmetic/multiply_operation.hpp>
#include <xmipp4/ops/arithmetic/negate_operation.hpp>
#include <xmipp4/ops/arithmetic/subtract_operation.hpp>

#include "fixtures/verb_dispatch_fixture.hpp"

using namespace xmipp4;
using namespace xmipp4::ops;
using xmipp4::test::verb_dispatch_fixture;

TEST_CASE_METHOD(
	verb_dispatch_fixture,
	"add dispatches an add_operation with both inputs in order",
	"[array_arithmetic]"
)
{
	check_binary_verb<add_operation>(add);
}

TEST_CASE_METHOD(
	verb_dispatch_fixture,
	"subtract dispatches a subtract_operation with both inputs in order",
	"[array_arithmetic]"
)
{
	check_binary_verb<subtract_operation>(subtract);
}

TEST_CASE_METHOD(
	verb_dispatch_fixture,
	"multiply dispatches a multiply_operation with both inputs in order",
	"[array_arithmetic]"
)
{
	check_binary_verb<multiply_operation>(multiply);
}

TEST_CASE_METHOD(
	verb_dispatch_fixture,
	"divide dispatches a divide_operation with both inputs in order",
	"[array_arithmetic]"
)
{
	check_binary_verb<divide_operation>(divide);
}

TEST_CASE_METHOD(
	verb_dispatch_fixture,
	"modulo dispatches a modulo_operation with both inputs in order",
	"[array_arithmetic]"
)
{
	check_binary_verb<modulo_operation>(modulo);
}

TEST_CASE_METHOD(
	verb_dispatch_fixture,
	"negate dispatches a negate_operation with its single input",
	"[array_arithmetic]"
)
{
	check_unary_verb<negate_operation>(negate);
}

TEST_CASE_METHOD(
	verb_dispatch_fixture,
	"abs dispatches an abs_operation with its single input",
	"[array_arithmetic]"
)
{
	check_unary_verb<abs_operation>(abs);
}
