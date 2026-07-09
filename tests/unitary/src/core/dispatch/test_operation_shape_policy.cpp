// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/dispatch/operation_shape_policy.hpp>

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers.hpp>
#include <catch2/matchers/catch_matchers_exception.hpp>

#include <stdexcept>
#include <vector>

using namespace xmipp4;
using namespace xmipp4::dispatch;

using shape_type = operation_shape_policy::shape_type;

namespace
{

class stub_shape_policy : public operation_shape_policy
{
public:
	using shape_type = operation_shape_policy::shape_type;

	void deduce(
		span<shape_type> /*canonical_output_shapes*/,
		span<const shape_type> /*input_shapes*/
	) const override {}
};

} // namespace

TEST_CASE(
	"operation_shape_policy::accept should succeed with zero outputs",
	"[operation_shape_policy]"
)
{
	const stub_shape_policy policy;
	CHECK_NOTHROW( policy.accept({}, {}, {}) );
}

TEST_CASE(
	"operation_shape_policy::accept should succeed when single user"
	" shape equals canonical",
	"[operation_shape_policy]"
)
{
	const stub_shape_policy policy;

	const std::vector<shape_type> user = { {3, 4} };
	const std::vector<shape_type> canonical = { {3, 4} };

	CHECK_NOTHROW( policy.accept(make_span(user), make_span(canonical), {}) );
}

TEST_CASE(
	"operation_shape_policy::accept should succeed when all user shapes"
	" equal canonical",
	"[operation_shape_policy]"
)
{
	const stub_shape_policy policy;

	const std::vector<shape_type> user = { {2, 3}, {4, 5, 6}, {} };
	const std::vector<shape_type> canonical = { {2, 3}, {4, 5, 6}, {} };

	CHECK_NOTHROW( policy.accept(make_span(user), make_span(canonical), {}) );
}

TEST_CASE(
	"operation_shape_policy::accept should throw when user shape at"
	" index 0 differs from canonical",
	"[operation_shape_policy]"
)
{
	const stub_shape_policy policy;

	const std::vector<shape_type> user = { {3, 4} };
	const std::vector<shape_type> canonical = { {3, 5} };

	REQUIRE_THROWS_MATCHES(
		policy.accept(make_span(user), make_span(canonical), {}),
		std::invalid_argument,
		Catch::Matchers::Message(
			"user-supplied output shape at index 0"
			" does not match the shape deduced from the inputs."
		)
	);
}

TEST_CASE(
	"operation_shape_policy::accept should throw when user shape at a"
	" later index differs from canonical",
	"[operation_shape_policy]"
)
{
	const stub_shape_policy policy;

	const std::vector<shape_type> user = { {2, 3}, {4, 5}, {1, 1} };
	const std::vector<shape_type> canonical = { {2, 3}, {4, 5}, {1, 9} };

	REQUIRE_THROWS_MATCHES(
		policy.accept(make_span(user), make_span(canonical), {}),
		std::invalid_argument,
		Catch::Matchers::Message(
			"user-supplied output shape at index 2"
			" does not match the shape deduced from the inputs."
		)
	);
}
