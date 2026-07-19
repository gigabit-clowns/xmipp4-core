// SPDX-License-Identifier: GPL-3.0-only

#include <catch2/catch_test_macros.hpp>

#include <backends/cpu/builders/type_dispatchers/homogeneous_type_dispatcher.hpp>

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

// Predicate rejecting the boolean type, accepting the rest.
template <typename T>
struct reject_boolean
	: std::integral_constant<bool, !std::is_same<T, bool>::value>
{
};

} // namespace

TEST_CASE(
	"homogeneous_type_dispatcher resolves the shared type once per operand",
	"[homogeneous_type_dispatcher]"
)
{
	mock_factory factory;

	REQUIRE_CALL(
		factory,
		call(
			std::vector<numerical_type>{ numerical_type::float32 },
			std::vector<numerical_type>{
				numerical_type::float32,
				numerical_type::float32
			}
		)
	).RETURN(nullptr);

	homogeneous_type_dispatcher<>::dispatch(
		factory,
		std::array<numerical_type, 1>{ numerical_type::float32 },
		std::array<numerical_type, 2>{
			numerical_type::float32,
			numerical_type::float32
		}
	);
}

TEST_CASE(
	"homogeneous_type_dispatcher throws when an input type differs",
	"[homogeneous_type_dispatcher]"
)
{
	using trompeloeil::_;

	mock_factory factory;
	FORBID_CALL(factory, call(_, _));

	CHECK_THROWS_AS(
		homogeneous_type_dispatcher<>::dispatch(
			factory,
			std::array<numerical_type, 1>{ numerical_type::float32 },
			std::array<numerical_type, 2>{
				numerical_type::float32,
				numerical_type::float64
			}
		),
		std::invalid_argument
	);
}

TEST_CASE(
	"homogeneous_type_dispatcher throws when an output type differs",
	"[homogeneous_type_dispatcher]"
)
{
	using trompeloeil::_;

	mock_factory factory;
	FORBID_CALL(factory, call(_, _));

	CHECK_THROWS_AS(
		homogeneous_type_dispatcher<>::dispatch(
			factory,
			std::array<numerical_type, 2>{
				numerical_type::float32,
				numerical_type::int32
			},
			std::array<numerical_type, 0>{}
		),
		std::invalid_argument
	);
}

TEST_CASE(
	"homogeneous_type_dispatcher invokes the factory when the predicate accepts "
	"the shared type",
	"[homogeneous_type_dispatcher]"
)
{
	mock_factory factory;

	REQUIRE_CALL(
		factory,
		call(
			std::vector<numerical_type>{ numerical_type::float32 },
			std::vector<numerical_type>{ numerical_type::float32 }
		)
	).RETURN(nullptr);

	homogeneous_type_dispatcher<reject_boolean>::dispatch(
		factory,
		std::array<numerical_type, 1>{ numerical_type::float32 },
		std::array<numerical_type, 1>{ numerical_type::float32 }
	);
}

TEST_CASE(
	"homogeneous_type_dispatcher throws without invoking the factory when the "
	"predicate rejects the shared type",
	"[homogeneous_type_dispatcher]"
)
{
	using trompeloeil::_;

	mock_factory factory;
	FORBID_CALL(factory, call(_, _));

	CHECK_THROWS_AS(
		homogeneous_type_dispatcher<reject_boolean>::dispatch(
			factory,
			std::array<numerical_type, 1>{ numerical_type::boolean },
			std::array<numerical_type, 1>{ numerical_type::boolean }
		),
		std::invalid_argument
	);
}
