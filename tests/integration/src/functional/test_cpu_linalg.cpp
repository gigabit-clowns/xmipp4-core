// SPDX-License-Identifier: GPL-3.0-only

#include <catch2/catch_test_macros.hpp>

#include <xmipp4/functional/linalg.hpp>

#include "fixtures/reduction_verb_fixture.hpp"

#include <xmipp4/ops/linalg/vecdot_operation.hpp>

#include <complex>
#include <cstddef>
#include <vector>

using namespace xmipp4;
using namespace xmipp4::ops;
using xmipp4::test::element_value;
using xmipp4::test::reduction_verb_fixture;

// vecdot is a broadcast followed by a sum along an axis, so it shares the
// reduction shape policy and the reduction loop with the reduction family.
// It is the only one of them taking two operands, which is what these cases
// are here to pin.

TEST_CASE_METHOD(
	reduction_verb_fixture,
	"vecdot contracts two vectors along their last axis",
	"[array_linalg][cpu]"
)
{
	auto left = make_sequence_operand<float32_t>({ 3 }, { 1, 2, 3 });
	auto right = make_sequence_operand<float32_t>({ 3 }, { 4, 5, 6 });
	const const_array_ref left_ref = left;
	const const_array_ref right_ref = right;

	check_values<float32_t>(
		xmipp4::vecdot(left_ref, right_ref, context, nullptr),
		{},
		{ 32 }
	);
}

TEST_CASE_METHOD(
	reduction_verb_fixture,
	"vecdot contracts a stack of vectors",
	"[array_linalg][cpu]"
)
{
	auto left = make_sequence_operand<float32_t>({ 2, 3 }, { 1, 2, 3, 4, 5, 6 });
	auto right = make_sequence_operand<float32_t>({ 2, 3 }, { 1, 0, 0, 0, 1, 0 });
	const const_array_ref left_ref = left;
	const const_array_ref right_ref = right;

	check_values<float32_t>(
		xmipp4::vecdot(left_ref, right_ref, context, nullptr),
		{ 2 },
		{ 1, 5 }
	);
}

TEST_CASE_METHOD(
	reduction_verb_fixture,
	"vecdot broadcasts the operand that is a single vector",
	"[array_linalg][cpu]"
)
{
	// Every other axis is a batch dimension, so a stack contracts with one
	// vector without that vector being written out.
	auto left = make_sequence_operand<float32_t>({ 2, 3 }, { 1, 2, 3, 4, 5, 6 });
	auto right = make_sequence_operand<float32_t>({ 1, 3 }, { 1, 1, 1 });
	const const_array_ref left_ref = left;
	const const_array_ref right_ref = right;

	check_values<float32_t>(
		xmipp4::vecdot(left_ref, right_ref, context, nullptr),
		{ 2 },
		{ 6, 15 }
	);
}

TEST_CASE_METHOD(
	reduction_verb_fixture,
	"vecdot contracts along an axis other than the last",
	"[array_linalg][cpu]"
)
{
	auto left = make_sequence_operand<float32_t>({ 2, 3 }, { 1, 2, 3, 4, 5, 6 });
	auto right = make_sequence_operand<float32_t>({ 2, 3 }, { 1, 1, 1, 2, 2, 2 });
	const const_array_ref left_ref = left;
	const const_array_ref right_ref = right;

	check_values<float32_t>(
		xmipp4::vecdot(left_ref, right_ref, 0, context, nullptr),
		{ 3 },
		{ 9, 12, 15 }
	);
}

TEST_CASE_METHOD(
	reduction_verb_fixture,
	"vecdot conjugates its first operand",
	"[array_linalg][cpu]"
)
{
	// A vector contracted with itself is then its squared magnitude, a real
	// quantity, rather than a complex number. This is what the array API and
	// NumPy's vecdot both do, and it is the only thing separating vecdot
	// from a plain sum of products.
	using complex_type = std::complex<float32_t>;

	auto operand = make_sequence_operand<complex_type>(
		{ 2 },
		{ element_value(3.0, 4.0), element_value(1.0, -2.0) }
	);
	const const_array_ref operand_ref = operand;

	const auto result =
		xmipp4::vecdot(operand_ref, operand_ref, context, nullptr);

	// |3 + 4i|^2 + |1 - 2i|^2 = 25 + 5, with no imaginary part left.
	const auto value = read_host<complex_type>(result, 1).front();
	CHECK( value.real() == 30.0F );
	CHECK( value.imag() == 0.0F );
}
