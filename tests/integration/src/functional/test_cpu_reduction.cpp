// SPDX-License-Identifier: GPL-3.0-only

#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>

#include <xmipp4/functional/reduction.hpp>

#include "fixtures/reduction_verb_fixture.hpp"

#include <xmipp4/ops/reduction/amax_operation.hpp>
#include <xmipp4/ops/reduction/amin_operation.hpp>
#include <xmipp4/ops/reduction/product_operation.hpp>
#include <xmipp4/ops/reduction/sum_operation.hpp>

#include <cmath>
#include <cstddef>
#include <limits>
#include <stdexcept>
#include <vector>

using namespace xmipp4;
using namespace xmipp4::ops;
using xmipp4::test::element_value;
using xmipp4::test::reduction_verb_fixture;

namespace
{

const double not_a_number = std::numeric_limits<double>::quiet_NaN();

// The input is filled with one value, so a sum is that value repeated. Taking
// it as one keeps the answer inside the narrowest type the operation admits,
// which is what lets the whole domain be swept from a single case.
template <typename T>
T repeated(T value, std::size_t count) noexcept
{
	return value * static_cast<T>(count);
}

} // anonymous namespace

TEST_CASE_METHOD(
	reduction_verb_fixture,
	"sum adds the elements along one axis",
	"[array_reduction][cpu]"
)
{
	// The last axis is the contiguous one, so this is the orientation whose
	// reduced elements are walked contiguously.
	check_reduction<sum_operation>(
		xmipp4::sum,
		{ 2 },
		false,
		element_value(1),
		[](auto x, std::size_t count) { return repeated(x, count); }
	);
}

TEST_CASE_METHOD(
	reduction_verb_fixture,
	"sum adds the elements along an outer axis",
	"[array_reduction][cpu]"
)
{
	// The surviving axes are the contiguous ones here, which is the other
	// orientation of the loop and the one a naive per-output pass gets wrong.
	check_reduction<sum_operation>(
		xmipp4::sum,
		{ 0 },
		false,
		element_value(1),
		[](auto x, std::size_t count) { return repeated(x, count); }
	);
}

TEST_CASE_METHOD(
	reduction_verb_fixture,
	"sum adds the elements along several axes at once",
	"[array_reduction][cpu]"
)
{
	check_reduction<sum_operation>(
		xmipp4::sum,
		{ 0, 2 },
		false,
		element_value(1),
		[](auto x, std::size_t count) { return repeated(x, count); }
	);
}

TEST_CASE_METHOD(
	reduction_verb_fixture,
	"sum adds every element when every axis is reduced",
	"[array_reduction][cpu]"
)
{
	// Leaves an output of rank zero, which is the shape the surviving space
	// collapses to.
	check_reduction<sum_operation>(
		xmipp4::sum,
		{ 0, 1, 2 },
		false,
		element_value(1),
		[](auto x, std::size_t count) { return repeated(x, count); }
	);
}

TEST_CASE_METHOD(
	reduction_verb_fixture,
	"sum leaves the array alone when no axis is reduced",
	"[array_reduction][cpu]"
)
{
	// An empty axis list is not a request to reduce everything, so this is a
	// copy rather than a total.
	check_reduction<sum_operation>(
		xmipp4::sum,
		{},
		false,
		element_value(1),
		[](auto x, std::size_t count) { return repeated(x, count); }
	);
}

TEST_CASE_METHOD(
	reduction_verb_fixture,
	"sum keeps the reduced axes at one when asked to",
	"[array_reduction][cpu]"
)
{
	check_reduction<sum_operation>(
		xmipp4::sum,
		{ 1 },
		true,
		element_value(1),
		[](auto x, std::size_t count) { return repeated(x, count); }
	);
}

TEST_CASE_METHOD(
	reduction_verb_fixture,
	"product multiplies the elements along an axis",
	"[array_reduction][cpu]"
)
{
	check_reduction<product_operation>(
		xmipp4::product,
		{ 1 },
		false,
		element_value(1),
		[](auto x, std::size_t) { return x; }
	);
}

TEST_CASE_METHOD(
	reduction_verb_fixture,
	"amax and amin carry a constant array through unchanged",
	"[array_reduction][cpu]"
)
{
	// Every element being equal, this pins the typing and the shape rather
	// than the choice itself, which the cases below make.
	check_reduction<amax_operation>(
		xmipp4::amax,
		{ 0, 2 },
		false,
		element_value(2),
		[](auto x, std::size_t) { return x; }
	);
	check_reduction<amin_operation>(
		xmipp4::amin,
		{ 0, 2 },
		false,
		element_value(2),
		[](auto x, std::size_t) { return x; }
	);
}

TEST_CASE_METHOD(
	reduction_verb_fixture,
	"product multiplies elements that differ",
	"[array_reduction][cpu]"
)
{
	// Two rows of three, reduced along the rows.
	const std::vector<std::size_t> extents = { 2, 3 };
	const std::vector<double> values = { 1, 2, 3, 4, 5, 6 };
	auto operand = make_sequence_operand<float32_t>(extents, values);
	const const_array_ref operand_ref = operand;

	const std::vector<std::ptrdiff_t> axes = { 0 };
	const auto result = xmipp4::product(
		operand_ref, make_span(axes), false, context, nullptr
	);

	check_values<float32_t>(result, { 3 }, { 4, 10, 18 });
}

TEST_CASE_METHOD(
	reduction_verb_fixture,
	"amax takes the largest element and amin the smallest",
	"[array_reduction][cpu]"
)
{
	const std::vector<std::size_t> extents = { 2, 3 };
	const std::vector<double> values = { 5, 1, 9, 3, 7, 2 };
	auto operand = make_sequence_operand<float32_t>(extents, values);
	const const_array_ref operand_ref = operand;

	SECTION( "along the axis the elements are strided over" )
	{
		const std::vector<std::ptrdiff_t> axes = { 0 };
		check_values<float32_t>(
			xmipp4::amax(operand_ref, make_span(axes), false, context, nullptr),
			{ 3 },
			{ 5, 7, 9 }
		);
		check_values<float32_t>(
			xmipp4::amin(operand_ref, make_span(axes), false, context, nullptr),
			{ 3 },
			{ 3, 1, 2 }
		);
	}

	SECTION( "along the axis the elements are contiguous over" )
	{
		const std::vector<std::ptrdiff_t> axes = { 1 };
		check_values<float32_t>(
			xmipp4::amax(operand_ref, make_span(axes), false, context, nullptr),
			{ 2 },
			{ 9, 7 }
		);
		check_values<float32_t>(
			xmipp4::amin(operand_ref, make_span(axes), false, context, nullptr),
			{ 2 },
			{ 1, 2 }
		);
	}
}

TEST_CASE_METHOD(
	reduction_verb_fixture,
	"amax and amin propagate an unordered element",
	"[array_reduction][cpu]"
)
{
	// std::max would propagate it only from one side, leaving the answer
	// dependent on where in the array it fell.
	const std::vector<std::size_t> extents = { 3 };
	const std::vector<std::ptrdiff_t> axes = { 0 };

	const auto values = GENERATE(
		std::vector<double>{ not_a_number, 1, 2 },
		std::vector<double>{ 1, not_a_number, 2 },
		std::vector<double>{ 1, 2, not_a_number }
	);

	auto operand = make_sequence_operand<float32_t>(extents, values);
	const const_array_ref operand_ref = operand;

	const auto largest =
		xmipp4::amax(operand_ref, make_span(axes), false, context, nullptr);
	const auto smallest =
		xmipp4::amin(operand_ref, make_span(axes), false, context, nullptr);

	CHECK( std::isnan(read_host<float32_t>(largest, 1).front()) );
	CHECK( std::isnan(read_host<float32_t>(smallest, 1).front()) );
}

TEST_CASE_METHOD(
	reduction_verb_fixture,
	"amax and amin reject a reduction over no elements",
	"[array_reduction][cpu]"
)
{
	// There is no largest element of nothing, and the lowest representable
	// value would be an invention rather than an answer. NumPy refuses the
	// same way.
	auto operand = make_sequence_operand<float32_t>({ 0, 3 }, {});
	const const_array_ref operand_ref = operand;
	const std::vector<std::ptrdiff_t> axes = { 0 };

	CHECK_THROWS_AS(
		xmipp4::amax(operand_ref, make_span(axes), false, context, nullptr),
		std::invalid_argument
	);
	CHECK_THROWS_AS(
		xmipp4::amin(operand_ref, make_span(axes), false, context, nullptr),
		std::invalid_argument
	);
}

TEST_CASE_METHOD(
	reduction_verb_fixture,
	"sum answers a reduction over no elements with zero",
	"[array_reduction][cpu]"
)
{
	// An empty sum has an answer where an empty extremum does not, which is
	// the whole of the difference between a fold with a neutral element and
	// one without.
	auto operand = make_sequence_operand<float32_t>({ 0, 3 }, {});
	const const_array_ref operand_ref = operand;
	const std::vector<std::ptrdiff_t> axes = { 0 };

	check_values<float32_t>(
		xmipp4::sum(operand_ref, make_span(axes), false, context, nullptr),
		{ 3 },
		{ 0, 0, 0 }
	);
}
