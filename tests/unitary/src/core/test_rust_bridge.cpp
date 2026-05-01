// SPDX-License-Identifier: GPL-3.0-only

#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>

#include <core/rust_bridge.hpp>

#include <xmipp4/core/multidimensional/strided_layout.hpp>
#include <xmipp4/core/numerical_type.hpp>

#include <tuple>
#include <vector>

using namespace xmipp4;
using namespace xmipp4::multidimensional;

#ifdef XMIPP4_CORE_ENABLE_RUST_BRIDGE

TEST_CASE("Rust promote_types bridge should match the C++ promote_types API", "[rust_bridge][numerical_type]")
{
	numerical_type first_type;
	numerical_type second_type;
	std::tie(first_type, second_type) = GENERATE(
		table<numerical_type, numerical_type>({
			{ numerical_type::boolean, numerical_type::int8 },
			{ numerical_type::char8, numerical_type::uint16 },
			{ numerical_type::int32, numerical_type::uint64 },
			{ numerical_type::float16, numerical_type::float64 },
			{ numerical_type::complex_float16, numerical_type::float32 },
			{ numerical_type::complex_float32, numerical_type::complex_float64 },
			{ numerical_type::unknown, numerical_type::float32 },
		})
	);

	const auto rust_promoted = static_cast<numerical_type>(
		xmipp4_rust_promote_types(
			static_cast<std::int32_t>(first_type),
			static_cast<std::int32_t>(second_type)
		)
	);

	CHECK( rust_promoted == promote_types(first_type, second_type) );
}

TEST_CASE("Rust storage-requirement bridge should match C++ strided layout", "[rust_bridge][strided_layout]")
{
	SECTION("contiguous layout")
	{
		const auto extents = std::vector<std::size_t>{4, 3, 2};
		const auto layout = strided_layout::make_contiguous_layout(make_span(extents));

		std::vector<std::size_t> layout_extents;
		std::vector<std::ptrdiff_t> layout_strides;
		layout.get_extents(layout_extents);
		layout.get_strides(layout_strides);

		const auto rust_requirement = xmipp4_rust_compute_storage_requirement(
			layout_extents.data(),
			layout_strides.data(),
			layout_extents.size(),
			layout.get_offset()
		);

		CHECK( rust_requirement == layout.compute_storage_requirement() );
	}

	SECTION("custom layout with non-zero offset")
	{
		const auto extents = std::vector<std::size_t>{2, 3};
		const auto strides = std::vector<std::ptrdiff_t>{5, 2};
		const auto layout = strided_layout::make_custom_layout(make_span(extents), make_span(strides), 7);

		const auto rust_requirement = xmipp4_rust_compute_storage_requirement(
			extents.data(),
			strides.data(),
			extents.size(),
			7
		);

		CHECK( rust_requirement == layout.compute_storage_requirement() );
	}
}

#else

TEST_CASE("Rust bridge tests are skipped when bridge support is disabled", "[rust_bridge]")
{
	SUCCEED();
}

#endif
