// SPDX-License-Identifier: GPL-3.0-only

#include <catch2/catch_test_macros.hpp>

#include <cpu/dispatch/helpers/type_and_inner_stride_dispatch.hpp>

#include <xmipp4/core/layout/access_layout.hpp>
#include <xmipp4/core/layout/access_layout_builder.hpp>
#include <xmipp4/core/numerical_type_dispatch.hpp>
#include <xmipp4/core/type_wrap.hpp>
#include <xmipp4/core/span.hpp>

#include <cstddef>
#include <cstdint>
#include <tuple>
#include <type_traits>
#include <utility>
#include <vector>

using namespace xmipp4;

namespace
{

/**
 * @brief Facts recorded by the probe callable at the innermost dispatch level.
 *
 * These are the only things this suite checks: whether the pieces are wired
 * together correctly. The behaviour of the underlying numerical-type and
 * stride dispatch is verified by their own tests.
 */
struct observations
{
	bool types_match = false;
	std::size_t number_of_strides = 0;
	bool first_stride_contiguous = false;
	const access_layout_implementation *layout_impl = nullptr;
};

/**
 * @brief Callable handed to dispatch_types_and_inner_strides.
 *
 * Records how it was invoked so the wiring can be inspected. It is templated
 * on the type_list the resolved types are expected to collapse to, so that
 * both the contents and the order of the bundled types are checked. The
 * returned observations also exercise the return-value plumbing of the
 * dispatch.
 */
template <typename ExpectedTypes>
struct probe
{
	int *calls;

	template <typename... Ts, typename... Strides>
	observations operator()(
		access_layout layout,
		type_list<Ts...>,
		std::tuple<Strides...> /*inner_strides*/
	) const
	{
		++(*calls);

		observations obs;
		obs.types_match =
			std::is_same<type_list<Ts...>, ExpectedTypes>::value;
		obs.number_of_strides = sizeof...(Strides);
		obs.first_stride_contiguous =
			is_first_contiguous(type_list<Strides...>());
		obs.layout_impl = layout.get_implementation();
		return obs;
	}

private:
	static bool is_first_contiguous(type_list<>) noexcept
	{
		return false;
	}

	template <typename Head, typename... Tail>
	static bool is_first_contiguous(type_list<Head, Tail...>) noexcept
	{
		return std::is_same<Head, contiguous_stride_tag>::value;
	}
};

/**
 * @brief Build a layout with the given iteration extents and one operand per
 * stride vector, keeping the strides verbatim.
 */
access_layout make_layout(
	const std::vector<std::size_t> &extents,
	const std::vector<std::vector<std::ptrdiff_t>> &operand_strides
)
{
	access_layout_builder builder;
	builder.set_extents(make_span(extents));
	for (const auto &strides : operand_strides)
	{
		builder.add_operand(make_span(extents), make_span(strides), 0);
	}
	return builder.build({});
}

} // namespace


TEST_CASE(
	"dispatch_types_and_inner_strides invokes the callable once with the "
	"resolved type and stride for a single operand",
	"[type_and_inner_stride_dispatch]"
)
{
	auto layout = make_layout({4}, {{1}});
	const auto *expected_impl = layout.get_implementation();

	int calls = 0;
	const auto obs = dispatch_types_and_inner_strides<1>(
		probe<type_list<float32_t>>{&calls},
		std::move(layout),
		numerical_type::float32
	);

	CHECK( calls == 1 );
	CHECK( obs.types_match );
	CHECK( obs.number_of_strides == 1 );
	CHECK( obs.first_stride_contiguous );
	// The very same layout instance is moved down to the callable.
	CHECK( obs.layout_impl == expected_impl );
}

TEST_CASE(
	"dispatch_types_and_inner_strides bundles several type axes into a "
	"type_list preserving their order",
	"[type_and_inner_stride_dispatch]"
)
{
	auto layout = make_layout({4}, {{1}, {1}});

	int calls = 0;
	const auto obs = dispatch_types_and_inner_strides<2>(
		probe<type_list<std::int32_t, float64_t>>{&calls},
		std::move(layout),
		numerical_type::int32,
		numerical_type::float64
	);

	CHECK( calls == 1 );
	CHECK( obs.types_match );
	CHECK( obs.number_of_strides == 2 );
}

TEST_CASE(
	"dispatch_types_and_inner_strides propagates the error from an "
	"unrecognized numerical type without invoking the callable",
	"[type_and_inner_stride_dispatch]"
)
{
	auto layout = make_layout({4}, {{1}});

	int calls = 0;
	CHECK_THROWS(
		dispatch_types_and_inner_strides<1>(
			probe<type_list<float32_t>>{&calls},
			std::move(layout),
			numerical_type::unknown
		)
	);
	CHECK( calls == 0 );
}

TEST_CASE(
	"dispatch_types_and_inner_strides sizes the stride tuple by the operand "
	"count independently of the number of type axes",
	"[type_and_inner_stride_dispatch]"
)
{
	auto layout = make_layout({4}, {{1}, {0}});

	int calls = 0;
	const auto obs = dispatch_types_and_inner_strides<2>(
		probe<type_list<float32_t>>{&calls},
		std::move(layout),
		numerical_type::float32
	);

	CHECK( calls == 1 );
	CHECK( obs.types_match );
	CHECK( obs.number_of_strides == 2 );
}
