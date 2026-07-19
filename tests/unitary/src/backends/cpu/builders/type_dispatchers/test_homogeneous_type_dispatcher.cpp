// SPDX-License-Identifier: GPL-3.0-only

#include <catch2/catch_test_macros.hpp>

#include <backends/cpu/builders/type_dispatchers/homogeneous_type_dispatcher.hpp>

#include <xmipp4/core/hardware/program.hpp>
#include <xmipp4/core/meta/type_list.hpp>
#include <xmipp4/core/numerical/numerical_type.hpp>
#include <xmipp4/core/numerical/numerical_type_traits.hpp>

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

// Factory mock. operator() reduces the resolved element type_lists to their
// runtime numerical_type so they can be matched with trompeloeil, forwarding
// to the mocked call.
class mock_factory
{
public:
	MAKE_MOCK2(
		call,
		std::shared_ptr<xmipp4::program>(
			std::vector<numerical_type>,
			std::vector<numerical_type>
		)
	);

	template <typename... Outs, typename... Ins>
	std::shared_ptr<xmipp4::program> operator()(
		type_list<Outs...>,
		type_list<Ins...>
	)
	{
		return call(
			std::vector<numerical_type>{ numerical_type_of<Outs>::value... },
			std::vector<numerical_type>{ numerical_type_of<Ins>::value... }
		);
	}
};

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
