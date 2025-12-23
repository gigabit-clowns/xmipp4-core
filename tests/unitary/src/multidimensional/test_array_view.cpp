// SPDX-License-Identifier: GPL-3.0-only

#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>

#include <xmipp4/core/multidimensional/array_view.hpp>

#include <xmipp4/core/multidimensional/array.hpp>
#include <xmipp4/core/multidimensional/array_descriptor.hpp>
#include <xmipp4/core/hardware/buffer.hpp>
#include <multidimensional/array_implementation.hpp>

#include <vector>

using namespace xmipp4;
using namespace xmipp4::multidimensional;

TEST_CASE("Default constructed array_view should have no storage", "[array_view]")
{
	array_view arr;
	CHECK( arr.get_storage() == nullptr );
	CHECK( arr.share_storage() == nullptr );
}

TEST_CASE("Default constructed array_view should have an empty descriptor", "[array_view]")
{
	array_view arr;
	CHECK( arr.get_descriptor() == array_descriptor() );
}

TEST_CASE("Constructing an array_view should store its attributes", "[array_view]")
{
	const auto data_type = GENERATE(
		numerical_type::int32,
		numerical_type::float64
	);

	const auto extents = GENERATE(
		std::vector<std::size_t>{1},
		std::vector<std::size_t>{20, 50}
	);

	const auto storage = GENERATE(
		std::make_shared<hardware::buffer>(
			nullptr, 
			1024, 
			hardware::get_host_memory_resource(), 
			nullptr
		),
		std::make_shared<hardware::buffer>(
			nullptr, 
			20, 
			hardware::get_host_memory_resource(), 
			nullptr
		)
	);

	const auto layout = strided_layout::make_contiguous_layout(make_span(extents));
	const array_descriptor descriptor(layout, data_type);

	array_view view;
	SECTION("from implementation")
	{
		view = array_view(
			std::make_shared<array_implementation>(storage, descriptor)
		);
	}
	SECTION("implicitly from another array")
	{
		view = array(storage, descriptor);
	}

	CHECK( view.get_storage() == storage.get() );
	CHECK( view.share_storage() == storage );
	CHECK( view.get_descriptor() == descriptor );
}	

TEST_CASE("Calling share on an array_view should return an array with the same content", "[array_view]")
{
	const auto data_type = GENERATE(
		numerical_type::int32,
		numerical_type::float64
	);

	const auto extents = GENERATE(
		std::vector<std::size_t>{1},
		std::vector<std::size_t>{20, 50}
	);

	const auto storage = GENERATE(
		std::make_shared<hardware::buffer>(
			nullptr, 
			1024, 
			hardware::get_host_memory_resource(), 
			nullptr
		),
		std::make_shared<hardware::buffer>(
			nullptr, 
			20, 
			hardware::get_host_memory_resource(), 
			nullptr
		)
	);

	const auto layout = strided_layout::make_contiguous_layout(make_span(extents));
	const array_descriptor descriptor(layout, data_type);
	const array_view view1(
		std::make_shared<array_implementation>(storage, descriptor)
	);
	const auto view2 = view1.share();

	CHECK( &view1.get_descriptor() == &view2.get_descriptor() );
	CHECK( view1.get_storage() == view2.get_storage() );
}	
