// SPDX-License-Identifier: GPL-3.0-only

#include <catch2/catch_test_macros.hpp>

#include <xmipp4/functional/reduction.hpp>

#include "fixtures/reduction_verb_fixture.hpp"

#include <xmipp4/ops/reduction/sum_operation.hpp>

#include <cstddef>

using namespace xmipp4;
using namespace xmipp4::ops;
using xmipp4::test::element_value;
using xmipp4::test::reduction_verb_fixture;

namespace
{

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
