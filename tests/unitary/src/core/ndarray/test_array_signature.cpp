// SPDX-License-Identifier: GPL-3.0-only

#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>

#include <xmipp4/core/ndarray/array_signature.hpp>
#include <xmipp4/core/ndarray/array.hpp>
#include <xmipp4/core/ndarray/array_view.hpp>
#include <xmipp4/core/hardware/memory_resource.hpp>

#include "../hardware/mock/mock_buffer.hpp"

#include <memory>
#include <vector>

using namespace xmipp4;
using namespace xmipp4::ndarray;
using namespace xmipp4::layout;

TEST_CASE("Default constructed array_signature should have empty descriptor and null memory resource", "[array_signature]")
{
	array_signature signature;
	CHECK( signature.get_descriptor() == array_descriptor() );
	CHECK( signature.get_memory_resource() == nullptr );
	CHECK( signature.get_layout() == strided_layout() );
	CHECK( signature.get_data_type() == numerical_type::unknown );
}

TEST_CASE("Constructing an array_signature with descriptor and memory resource should store its attributes", "[array_signature]")
{
	const std::vector<std::size_t> extents = {20, 30, 2, 16};
	const auto layout = strided_layout::make_contiguous_layout(make_span(extents));
	const auto data_type = numerical_type::float32;
	const array_descriptor descriptor(layout, data_type);
	const auto* resource = &hardware::get_host_memory_resource();

	array_signature signature(descriptor, resource);
	CHECK( signature.get_descriptor() == descriptor );
	CHECK( signature.get_memory_resource() == resource );
}

TEST_CASE("Constructing an array_signature with layout and data type should store its attributes", "[array_signature]")
{
	const std::vector<std::size_t> extents = {20, 30, 2, 16};
	const auto layout = strided_layout::make_contiguous_layout(make_span(extents));
	const auto data_type = numerical_type::float32;
	const auto* resource = &hardware::get_host_memory_resource();

	array_signature signature(layout, data_type, resource);
	CHECK( signature.get_layout() == layout );
	CHECK( signature.get_data_type() == data_type );
	CHECK( signature.get_memory_resource() == resource );
}

TEST_CASE("Array signatures should be equal if the descriptor and memory resource are equal", "[array_signature]")
{
	const std::vector<std::size_t> extents = {20, 30, 2, 16};
	const auto layout = strided_layout::make_contiguous_layout(make_span(extents));
	const auto data_type = numerical_type::float32;
	const array_descriptor descriptor(layout, data_type);
	const auto* resource = &hardware::get_host_memory_resource();

	const array_signature signature1(descriptor, resource);
	const array_signature signature2(descriptor, resource);
	
	CHECK( signature1 == signature2 );
	CHECK( (signature1 != signature2) == false );
}

TEST_CASE("Array signatures should be unequal if descriptor or memory resource mismatch", "[array_signature]")
{
	const std::vector<std::size_t> extents1 = {20, 30, 2, 16};
	const std::vector<std::size_t> extents2 = {20, 30, 3, 16};
	const auto layout1 = strided_layout::make_contiguous_layout(make_span(extents1));
	const auto layout2 = strided_layout::make_contiguous_layout(make_span(extents2));
	const auto data_type = numerical_type::float32;
	const auto* resource = &hardware::get_host_memory_resource();

	const array_signature signature1(layout1, data_type, resource);
	const array_signature signature2(layout2, data_type, resource);
	const array_signature signature3(layout1, data_type, nullptr);

	CHECK( signature1 != signature2 );
	CHECK( signature1 != signature3 );
	CHECK( signature2 != signature3 );
}

TEST_CASE("hash value of two equal array signatures should be equal", "[array_signature]")
{
	const std::vector<std::size_t> extents = {20, 30, 2, 16};
	const auto layout = strided_layout::make_contiguous_layout(make_span(extents));
	const auto data_type = numerical_type::float32;
	const auto* resource = &hardware::get_host_memory_resource();

	const array_signature signature1(layout, data_type, resource);
	const array_signature signature2(layout, data_type, resource);
	
	CHECK( signature1.hash() == signature2.hash() );
}

TEST_CASE("hash value of array signatures should be different for different descriptors or memory resources", "[array_signature]")
{
	const std::vector<std::size_t> extents = {20, 30, 2, 16};
	const auto layout = strided_layout::make_contiguous_layout(make_span(extents));
	const auto data_type = numerical_type::float32;
	const auto* resource = &hardware::get_host_memory_resource();

	const array_signature signature1(layout, data_type, resource);
	const array_signature signature2(layout, data_type, nullptr);
	const array_signature signature3(layout, numerical_type::int32, resource);
	
	CHECK( signature1.hash() != signature2.hash() );
	CHECK( signature1.hash() != signature3.hash() );
}

TEST_CASE("setters in array_signature should update its attributes", "[array_signature]")
{
	array_signature signature;
	const std::vector<std::size_t> extents = {20, 30, 2, 16};
	const auto layout = strided_layout::make_contiguous_layout(make_span(extents));
	const auto data_type = numerical_type::float32;
	const array_descriptor descriptor(layout, data_type);
	const auto* resource = &hardware::get_host_memory_resource();

	signature.set_descriptor(descriptor);
	signature.set_memory_resource(resource);

	CHECK( signature.get_descriptor() == descriptor );
	CHECK( signature.get_memory_resource() == resource );
}

TEST_CASE("from_array in array_signature should correctly construct from an array and array_view", "[array_signature]")
{
	const std::vector<std::size_t> extents = {20, 30};
	const auto layout = strided_layout::make_contiguous_layout(make_span(extents));
	const auto data_type = numerical_type::float32;
	const array_descriptor descriptor(layout, data_type);
	auto& resource = hardware::get_host_memory_resource();
	
	const auto storage = std::make_shared<hardware::mock_buffer>();
	ALLOW_CALL(std::as_const(*storage), get_memory_resource())
		.LR_RETURN(resource);

	array arr(storage, descriptor);
	const auto signature1 = array_signature::from_array(arr);
	CHECK( signature1.get_descriptor() == descriptor );
	CHECK( signature1.get_memory_resource() == &resource );

	array_view view = arr;
	const auto signature2 = array_signature::from_array(view);
	CHECK( signature2.get_descriptor() == descriptor );
	CHECK( signature2.get_memory_resource() == &resource );
}
