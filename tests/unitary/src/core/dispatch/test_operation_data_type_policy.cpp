// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/dispatch/operation_data_type_policy.hpp>

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers.hpp>
#include <catch2/matchers/catch_matchers_exception.hpp>

#include <stdexcept>
#include <vector>

using namespace xmipp4;

namespace
{

class stub_data_type_policy : public operation_data_type_policy
{
public:
	void deduce(
		span<numerical_type> /*canonical_output_types*/,
		span<const numerical_type> /*input_types*/
	) const override {}
};

} // namespace

TEST_CASE(
	"operation_data_type_policy::accept should succeed with zero outputs",
	"[operation_data_type_policy]"
)
{
	const stub_data_type_policy policy;
	CHECK_NOTHROW( policy.accept({}, {}, {}) );
}

TEST_CASE(
	"operation_data_type_policy::accept should succeed when single user"
	" type equals canonical",
	"[operation_data_type_policy]"
)
{
	const stub_data_type_policy policy;

	const std::vector<numerical_type> user = { numerical_type::float32 };
	const std::vector<numerical_type> canonical = { numerical_type::float32 };

	CHECK_NOTHROW( policy.accept(make_span(user), make_span(canonical), {}) );
}

TEST_CASE(
	"operation_data_type_policy::accept should succeed when all user types"
	" equal canonical",
	"[operation_data_type_policy]"
)
{
	const stub_data_type_policy policy;

	const std::vector<numerical_type> user = {
		numerical_type::float32, numerical_type::int32
	};
	const std::vector<numerical_type> canonical = {
		numerical_type::float32, numerical_type::int32
	};

	CHECK_NOTHROW( policy.accept(make_span(user), make_span(canonical), {}) );
}

TEST_CASE(
	"operation_data_type_policy::accept should throw when user type at"
	" index 0 differs from canonical",
	"[operation_data_type_policy]"
)
{
	const stub_data_type_policy policy;

	const std::vector<numerical_type> user = { numerical_type::float32 };
	const std::vector<numerical_type> canonical = { numerical_type::float64 };

	REQUIRE_THROWS_MATCHES(
		policy.accept(make_span(user), make_span(canonical), {}),
		std::invalid_argument,
		Catch::Matchers::Message(
			"user-supplied output data type at index 0 (float32)"
			" does not match the data type deduced from the inputs (float64)."
		)
	);
}

TEST_CASE(
	"operation_data_type_policy::accept should throw when user type at a"
	" later index differs from canonical",
	"[operation_data_type_policy]"
)
{
	const stub_data_type_policy policy;

	const std::vector<numerical_type> user = {
		numerical_type::float32,
		numerical_type::int32,
		numerical_type::uint8
	};
	const std::vector<numerical_type> canonical = {
		numerical_type::float32,
		numerical_type::int32,
		numerical_type::int64
	};

	REQUIRE_THROWS_MATCHES(
		policy.accept(make_span(user), make_span(canonical), {}),
		std::invalid_argument,
		Catch::Matchers::Message(
			"user-supplied output data type at index 2 (uint8)"
			" does not match the data type deduced from the inputs (int64)."
		)
	);
}
