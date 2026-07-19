// SPDX-License-Identifier: GPL-3.0-only

#include <catch2/catch_test_macros.hpp>

#include <backends/cpu/builders/type_dispatchers/independent_type_dispatcher.hpp>

#include <xmipp4/core/numerical/numerical_type.hpp>

#include "mock/mock_factory.hpp"

#include <trompeloeil.hpp>

#include <array>
#include <memory>
#include <stdexcept>
#include <type_traits>
#include <vector>

using namespace xmipp4;
using namespace xmipp4::cpu;

namespace
{

// Predicate accepting a (output, input) pair when the output is constructible
// from the input, mirroring the copy operation's admissibility.
template <typename T, typename Q>
struct is_convertible_pair : std::is_constructible<T, Q> {};

} // namespace

TEST_CASE(
	"independent_type_dispatcher resolves each operand type independently",
	"[independent_type_dispatcher]"
)
{
	mock_factory factory;

	// The output and input types need not match.
	REQUIRE_CALL(
		factory,
		call(
			std::vector<numerical_type>{ numerical_type::int32 },
			std::vector<numerical_type>{ numerical_type::float32 }
		)
	).RETURN(nullptr);

	independent_type_dispatcher<>::dispatch(
		factory,
		std::array<numerical_type, 1>{ numerical_type::int32 },
		std::array<numerical_type, 1>{ numerical_type::float32 }
	);
}

TEST_CASE(
	"independent_type_dispatcher invokes the factory when the predicate accepts "
	"the type combination",
	"[independent_type_dispatcher]"
)
{
	mock_factory factory;

	// A complex output is constructible from a real input.
	REQUIRE_CALL(
		factory,
		call(
			std::vector<numerical_type>{ numerical_type::complex_float32 },
			std::vector<numerical_type>{ numerical_type::float32 }
		)
	).RETURN(nullptr);

	independent_type_dispatcher<is_convertible_pair>::dispatch(
		factory,
		std::array<numerical_type, 1>{ numerical_type::complex_float32 },
		std::array<numerical_type, 1>{ numerical_type::float32 }
	);
}

TEST_CASE(
	"independent_type_dispatcher throws without invoking the factory when the "
	"predicate rejects the type combination",
	"[independent_type_dispatcher]"
)
{
	using trompeloeil::_;

	mock_factory factory;
	FORBID_CALL(factory, call(_, _));

	// A real output can not be constructed from a complex input.
	CHECK_THROWS_AS(
		independent_type_dispatcher<is_convertible_pair>::dispatch(
			factory,
			std::array<numerical_type, 1>{ numerical_type::float32 },
			std::array<numerical_type, 1>{ numerical_type::complex_float32 }
		),
		std::invalid_argument
	);
}
