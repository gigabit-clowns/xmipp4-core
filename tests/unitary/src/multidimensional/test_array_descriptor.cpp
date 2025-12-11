// SPDX-License-Identifier: GPL-3.0-only

#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>

#include <xmipp4/core/multidimensional/array_descriptor.hpp>

using namespace xmipp4;
using namespace xmipp4::multidimensional;

TEST_CASE("Default constructed array_descriptor should have an empty layout and unknown numerical type", "[array_descriptor]")
{
	array_descriptor descriptor;
	CHECK( descriptor.get_layout() == strided_layout() );
	CHECK( descriptor.get_data_type() == numerical_type::unknown );
}

TEST_CASE("Constructing an array_descriptor should store its attributes", "[array_descriptor]")
{
	const auto data_type = GENERATE(
		numerical_type::int16,
		numerical_type::float32
	);
	const auto extents = GENERATE(
		std::vector<std::size_t>{1, 2, 3},
		std::vector<std::size_t>{20, 30, 2, 16}
	);

	const auto layout = strided_layout::make_contiguous_layout(make_span(extents));

	array_descriptor descriptor(layout, data_type);
	CHECK( descriptor.get_layout() == layout );
	CHECK( descriptor.get_data_type() == data_type );
}
