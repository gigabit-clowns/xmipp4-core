// SPDX-License-Identifier: GPL-3.0-only

#include <catch2/catch_test_macros.hpp>

#include <em/image/formats/mrc/mrc_region_transfer.hpp>

#include <rexlib/core/exceptions/invalid_operation_error.hpp>
#include <rexlib/core/numerical/numerical_type_traits.hpp>
#include <rexlib/em/image/image_transfer_plan.hpp>

#include <algorithm>
#include <complex>
#include <cstring>
#include <stdexcept>
#include <vector>

using namespace rexlib;
using namespace rexlib::em;
using namespace rexlib::em::mrc;

namespace
{

// Reverses the bytes of a value without going through the production helper,
// so that a test of the byte order does not agree with it by construction.
template <typename T>
T reversed(T value)
{
	unsigned char raw[sizeof(T)];
	std::memcpy(raw, &value, sizeof(raw));
	std::reverse(raw, raw + sizeof(raw));

	T result;
	std::memcpy(&result, raw, sizeof(result));
	return result;
}

template <typename T>
std::vector<T> counting(std::size_t count, T first = T())
{
	std::vector<T> values(count);
	for (std::size_t i = 0; i < count; ++i)
	{
		values[i] = static_cast<T>(first + static_cast<T>(i));
	}

	return values;
}

std::vector<std::ptrdiff_t>
contiguous_strides(const std::vector<std::size_t> &extents)
{
	std::vector<std::ptrdiff_t> strides(extents.size());

	std::ptrdiff_t stride = 1;
	for (auto axis = extents.size(); axis > 0; --axis)
	{
		strides[axis - 1] = stride;
		stride *= static_cast<std::ptrdiff_t>(extents[axis - 1]);
	}

	return strides;
}

byte_order other_byte_order()
{
	return get_system_byte_order() == byte_order::little_endian
		? byte_order::big_endian
		: byte_order::little_endian;
}

template <typename T>
const byte* as_file(const std::vector<T> &values)
{
	return reinterpret_cast<const byte*>(values.data());
}

template <typename T>
byte* as_file(std::vector<T> &values)
{
	return reinterpret_cast<byte*>(values.data());
}

} // anonymous namespace

TEST_CASE( "one region is moved out of a file and into an array",
	"[mrc_region_transfer]" )
{
	const std::vector<std::size_t> extents = {3, 4};
	const auto strides = contiguous_strides(extents);
	const auto file = counting<float32_t>(12);

	image_transfer_plan regions(make_span(extents), 2, 2);
	regions.add(make_span(std::vector<std::size_t>{0, 0}),
		make_span(std::vector<std::size_t>{0, 0}));

	const mrc_region_transfer transfer(
		regions,
		make_span(extents), make_span(strides),
		make_span(extents), make_span(strides),
		0
	);

	SECTION( "every value arrives where it belongs" )
	{
		std::vector<float32_t> array(12, -1.0F);
		transfer.read(
			array.data(), numerical_type::float32,
			as_file(file), numerical_type::float32,
			get_system_byte_order()
		);

		REQUIRE( array == file );
	}

	SECTION( "the transfer reports how many regions it holds" )
	{
		REQUIRE( transfer.get_region_count() == 1 );
	}
}

TEST_CASE( "a batch of regions shares one layout",
	"[mrc_region_transfer]" )
{
	const std::vector<std::size_t> file_extents = {3, 2, 2};
	const std::vector<std::size_t> region_extents = {2, 2};
	const auto file_strides = contiguous_strides(file_extents);
	const auto file = counting<float32_t>(12);

	image_transfer_plan regions(make_span(region_extents), 3, 3);
	for (std::size_t i = 0; i < 3; ++i)
	{
		regions.add(
			make_span(std::vector<std::size_t>{i, 0, 0}),
			make_span(std::vector<std::size_t>{2 - i, 0, 0})
		);
	}

	const mrc_region_transfer transfer(
		regions,
		make_span(file_extents), make_span(file_strides),
		make_span(file_extents), make_span(file_strides),
		0
	);

	SECTION( "each region lands where its own offsets put it" )
	{
		std::vector<float32_t> array(12, -1.0F);
		transfer.read(
			array.data(), numerical_type::float32,
			as_file(file), numerical_type::float32,
			get_system_byte_order()
		);

		const std::vector<float32_t> expected = {
			8, 9, 10, 11, 4, 5, 6, 7, 0, 1, 2, 3
		};

		REQUIRE( array == expected );
		REQUIRE( transfer.get_region_count() == 3 );
	}
}

TEST_CASE( "a region reaches an array of a different rank",
	"[mrc_region_transfer]" )
{
	const std::vector<std::size_t> file_extents = {3, 2, 2};
	const std::vector<std::size_t> array_extents = {2, 2};
	const std::vector<std::size_t> region_extents = {2, 2};
	const auto file_strides = contiguous_strides(file_extents);
	const auto array_strides = contiguous_strides(array_extents);
	const auto file = counting<float32_t>(12);

	image_transfer_plan regions(make_span(region_extents), 3, 2);
	regions.add(make_span(std::vector<std::size_t>{1, 0, 0}),
		make_span(std::vector<std::size_t>{0, 0}));

	const mrc_region_transfer transfer(
		regions,
		make_span(file_extents), make_span(file_strides),
		make_span(array_extents), make_span(array_strides),
		0
	);

	std::vector<float32_t> array(4, -1.0F);
	transfer.read(
		array.data(), numerical_type::float32,
		as_file(file), numerical_type::float32,
		get_system_byte_order()
	);

	REQUIRE( array == std::vector<float32_t>{4, 5, 6, 7} );
}

TEST_CASE( "a region lands in a strided array",
	"[mrc_region_transfer]" )
{
	const std::vector<std::size_t> extents = {2, 3};
	const std::vector<std::ptrdiff_t> file_strides = {3, 1};
	// A (2,3) window of a (2,5) buffer, starting one element in.
	const std::vector<std::ptrdiff_t> array_strides = {5, 1};
	const auto file = counting<float32_t>(6);

	image_transfer_plan regions(make_span(extents), 2, 2);
	regions.add(make_span(std::vector<std::size_t>{0, 0}),
		make_span(std::vector<std::size_t>{0, 0}));

	const mrc_region_transfer transfer(
		regions,
		make_span(extents), make_span(file_strides),
		make_span(extents), make_span(array_strides),
		1
	);

	std::vector<float32_t> array(10, -1.0F);
	transfer.read(
		array.data(), numerical_type::float32,
		as_file(file), numerical_type::float32,
		get_system_byte_order()
	);

	const std::vector<float32_t> expected = {
		-1, 0, 1, 2, -1,
		-1, 3, 4, 5, -1
	};

	REQUIRE( array == expected );
}

TEST_CASE( "values are converted into the type asked for",
	"[mrc_region_transfer]" )
{
	const std::vector<std::size_t> extents = {2, 2};
	const auto strides = contiguous_strides(extents);

	image_transfer_plan regions(make_span(extents), 2, 2);
	regions.add(make_span(std::vector<std::size_t>{0, 0}),
		make_span(std::vector<std::size_t>{0, 0}));

	const mrc_region_transfer transfer(
		regions,
		make_span(extents), make_span(strides),
		make_span(extents), make_span(strides),
		0
	);

	SECTION( "a narrower integer widens into a float" )
	{
		const std::vector<std::int16_t> file = {-2, -1, 0, 1};
		std::vector<float32_t> array(4, 9.0F);

		transfer.read(
			array.data(), numerical_type::float32,
			as_file(file), numerical_type::int16,
			get_system_byte_order()
		);

		REQUIRE( array == std::vector<float32_t>{-2, -1, 0, 1} );
	}

	SECTION( "half precision widens into a double" )
	{
		// Every one of these is exact in half precision, so widening them
		// loses nothing and the comparison can be an equality.
		const std::vector<float16_t> file = {
			float16_t(0.5F), float16_t(1.5F),
			float16_t(-2.5F), float16_t(3.0F)
		};
		std::vector<float64_t> array(4, 9.0);

		transfer.read(
			array.data(), numerical_type::float64,
			as_file(file), numerical_type::float16,
			get_system_byte_order()
		);

		REQUIRE( array == std::vector<float64_t>{0.5, 1.5, -2.5, 3.0} );
	}

	SECTION( "a double narrows into half precision" )
	{
		const std::vector<float64_t> array = {0.5, 1.5, -2.5, 3.0};
		std::vector<float16_t> file(4);

		transfer.write(
			array.data(), numerical_type::float64,
			as_file(file), numerical_type::float16,
			get_system_byte_order()
		);

		REQUIRE( static_cast<float>(file[0]) == 0.5F );
		REQUIRE( static_cast<float>(file[2]) == -2.5F );
	}

	SECTION( "a real value becomes the real part of a complex one" )
	{
		const std::vector<float32_t> file = {1, 2, 3, 4};
		std::vector<std::complex<float32_t>> array(4);

		transfer.read(
			array.data(), numerical_type::complex_float32,
			as_file(file), numerical_type::float32,
			get_system_byte_order()
		);

		REQUIRE( array[0] == std::complex<float32_t>(1.0F, 0.0F) );
		REQUIRE( array[3] == std::complex<float32_t>(4.0F, 0.0F) );
	}

	SECTION( "a complex value has no real destination" )
	{
		const std::vector<std::complex<float32_t>> file(4);
		std::vector<float32_t> array(4);

		REQUIRE_THROWS_AS(
			transfer.read(
				array.data(), numerical_type::float32,
				as_file(file), numerical_type::complex_float32,
				get_system_byte_order()
			),
			invalid_operation_error
		);
	}

	SECTION( "a type no MRC file holds is refused" )
	{
		const std::vector<float64_t> file(4);
		std::vector<float64_t> array(4);

		REQUIRE_THROWS_AS(
			transfer.read(
				array.data(), numerical_type::float64,
				as_file(file), numerical_type::float64,
				get_system_byte_order()
			),
			invalid_operation_error
		);
	}
}

TEST_CASE( "a file of the other byte order is read in it",
	"[mrc_region_transfer]" )
{
	const std::vector<std::size_t> extents = {2, 2};
	const auto strides = contiguous_strides(extents);

	image_transfer_plan regions(make_span(extents), 2, 2);
	regions.add(make_span(std::vector<std::size_t>{0, 0}),
		make_span(std::vector<std::size_t>{0, 0}));

	const mrc_region_transfer transfer(
		regions,
		make_span(extents), make_span(strides),
		make_span(extents), make_span(strides),
		0
	);

	SECTION( "reversed integers arrive as themselves" )
	{
		const std::vector<std::int16_t> values = {-2, -1, 3, 1000};
		std::vector<std::int16_t> file(values.size());
		std::transform(values.begin(), values.end(), file.begin(),
			[] (std::int16_t v) { return reversed(v); });

		std::vector<std::int16_t> array(4, 0);
		transfer.read(
			array.data(), numerical_type::int16,
			as_file(file), numerical_type::int16,
			other_byte_order()
		);

		REQUIRE( array == values );
	}

	SECTION( "reversed floats arrive as themselves" )
	{
		const std::vector<float32_t> values = {1.5F, -2.25F, 0.0F, 1024.0F};
		std::vector<float32_t> file(values.size());
		std::transform(values.begin(), values.end(), file.begin(),
			[] (float32_t v) { return reversed(v); });

		std::vector<float32_t> array(4, 0.0F);
		transfer.read(
			array.data(), numerical_type::float32,
			as_file(file), numerical_type::float32,
			other_byte_order()
		);

		REQUIRE( array == values );
	}

	SECTION( "each component of a complex value is reversed on its own" )
	{
		const std::vector<std::complex<float32_t>> values = {
			{1.5F, -2.5F}, {0.0F, 1.0F}, {3.0F, 4.0F}, {-1.0F, 0.5F}
		};
		std::vector<std::complex<float32_t>> file(values.size());
		std::transform(values.begin(), values.end(), file.begin(),
			[] (const std::complex<float32_t> &v)
			{
				return std::complex<float32_t>(
					reversed(v.real()), reversed(v.imag())
				);
			});

		std::vector<std::complex<float32_t>> array(4);
		transfer.read(
			array.data(), numerical_type::complex_float32,
			as_file(file), numerical_type::complex_float32,
			other_byte_order()
		);

		REQUIRE( array == values );
	}

	SECTION( "single bytes are the same in either order" )
	{
		const std::vector<std::int8_t> file = {-2, -1, 0, 1};
		std::vector<std::int8_t> array(4, 9);

		transfer.read(
			array.data(), numerical_type::int8,
			as_file(file), numerical_type::int8,
			other_byte_order()
		);

		REQUIRE( array == file );
	}
}

TEST_CASE( "a region is moved out of an array and into a file",
	"[mrc_region_transfer]" )
{
	const std::vector<std::size_t> extents = {2, 2};
	const auto strides = contiguous_strides(extents);

	image_transfer_plan regions(make_span(extents), 2, 2);
	regions.add(make_span(std::vector<std::size_t>{0, 0}),
		make_span(std::vector<std::size_t>{0, 0}));

	const mrc_region_transfer transfer(
		regions,
		make_span(extents), make_span(strides),
		make_span(extents), make_span(strides),
		0
	);

	SECTION( "values are converted into what the file holds" )
	{
		const std::vector<float32_t> array = {-2.0F, -1.0F, 0.0F, 1.0F};
		std::vector<std::int16_t> file(4, 9);

		transfer.write(
			array.data(), numerical_type::float32,
			as_file(file), numerical_type::int16,
			get_system_byte_order()
		);

		REQUIRE( file == std::vector<std::int16_t>{-2, -1, 0, 1} );
	}

	SECTION( "a file of the other byte order is written in it" )
	{
		const std::vector<float32_t> array = {-2.0F, -1.0F, 0.0F, 1.0F};
		std::vector<std::int16_t> file(4, 9);

		transfer.write(
			array.data(), numerical_type::float32,
			as_file(file), numerical_type::int16,
			other_byte_order()
		);

		// The bytes are reversed in the type the file holds, which is what
		// makes a wider array type land as the narrower value it converts to
		// rather than as a reversal of the original.
		const std::vector<std::int16_t> expected = {
			reversed<std::int16_t>(-2), reversed<std::int16_t>(-1),
			reversed<std::int16_t>(0), reversed<std::int16_t>(1)
		};

		REQUIRE( file == expected );
	}

	SECTION( "what is written is read back unchanged" )
	{
		const std::vector<float32_t> array = {1.5F, -2.25F, 0.0F, 8.0F};
		std::vector<float32_t> file(4, 9.0F);
		std::vector<float32_t> reread(4, 0.0F);

		transfer.write(
			array.data(), numerical_type::float32,
			as_file(file), numerical_type::float32,
			other_byte_order()
		);
		transfer.read(
			reread.data(), numerical_type::float32,
			as_file(file), numerical_type::float32,
			other_byte_order()
		);

		REQUIRE( reread == array );
	}

	SECTION( "a type the file can not hold is refused" )
	{
		const std::vector<std::complex<float32_t>> array(4);
		std::vector<float32_t> file(4);

		REQUIRE_THROWS_AS(
			transfer.write(
				array.data(), numerical_type::complex_float32,
				as_file(file), numerical_type::float32,
				get_system_byte_order()
			),
			invalid_operation_error
		);
	}
}

TEST_CASE( "a batch that does not fit is refused before anything moves",
	"[mrc_region_transfer]" )
{
	const std::vector<std::size_t> extents = {2, 2};
	const std::vector<std::size_t> region_extents = {2, 2};
	const auto strides = contiguous_strides(extents);

	SECTION( "a region past the end of the file is refused" )
	{
		image_transfer_plan regions(make_span(region_extents), 2, 2);
		regions.add(make_span(std::vector<std::size_t>{1, 0}),
			make_span(std::vector<std::size_t>{0, 0}));

		REQUIRE_THROWS_AS(
			mrc_region_transfer(
				regions,
				make_span(extents), make_span(strides),
				make_span(extents), make_span(strides),
				0
			),
			std::out_of_range
		);
	}

	SECTION( "a region past the end of the array is refused" )
	{
		image_transfer_plan regions(make_span(region_extents), 2, 2);
		regions.add(make_span(std::vector<std::size_t>{0, 0}),
			make_span(std::vector<std::size_t>{0, 1}));

		REQUIRE_THROWS_AS(
			mrc_region_transfer(
				regions,
				make_span(extents), make_span(strides),
				make_span(extents), make_span(strides),
				0
			),
			std::out_of_range
		);
	}

	SECTION( "one bad region among good ones refuses the whole batch" )
	{
		image_transfer_plan regions(make_span(region_extents), 2, 2);
		regions.add(make_span(std::vector<std::size_t>{0, 0}),
			make_span(std::vector<std::size_t>{0, 0}));
		regions.add(make_span(std::vector<std::size_t>{0, 3}),
			make_span(std::vector<std::size_t>{0, 0}));

		REQUIRE_THROWS_AS(
			mrc_region_transfer(
				regions,
				make_span(extents), make_span(strides),
				make_span(extents), make_span(strides),
				0
			),
			std::out_of_range
		);
	}
}

TEST_CASE( "a batch whose ranks disagree with its sides is refused",
	"[mrc_region_transfer]" )
{
	const std::vector<std::size_t> extents = {2, 2};
	const std::vector<std::size_t> deeper = {1, 2, 2};
	const auto strides = contiguous_strides(extents);
	const auto deeper_strides = contiguous_strides(deeper);

	image_transfer_plan regions(make_span(extents), 2, 2);
	regions.add(make_span(std::vector<std::size_t>{0, 0}),
		make_span(std::vector<std::size_t>{0, 0}));

	SECTION( "file extents of the wrong rank are refused" )
	{
		REQUIRE_THROWS_AS(
			mrc_region_transfer(
				regions,
				make_span(deeper), make_span(deeper_strides),
				make_span(extents), make_span(strides),
				0
			),
			std::invalid_argument
		);
	}

	SECTION( "array extents of the wrong rank are refused" )
	{
		REQUIRE_THROWS_AS(
			mrc_region_transfer(
				regions,
				make_span(extents), make_span(strides),
				make_span(deeper), make_span(deeper_strides),
				0
			),
			std::invalid_argument
		);
	}

	SECTION( "strides that do not match their extents are refused" )
	{
		REQUIRE_THROWS_AS(
			mrc_region_transfer(
				regions,
				make_span(extents), make_span(deeper_strides),
				make_span(extents), make_span(strides),
				0
			),
			std::invalid_argument
		);
	}
}

TEST_CASE( "an empty batch moves nothing and succeeds",
	"[mrc_region_transfer]" )
{
	const std::vector<std::size_t> extents = {2, 2};
	const auto strides = contiguous_strides(extents);
	const std::vector<float32_t> file = {1, 2, 3, 4};

	const image_transfer_plan regions(make_span(extents), 2, 2);
	const mrc_region_transfer transfer(
		regions,
		make_span(extents), make_span(strides),
		make_span(extents), make_span(strides),
		0
	);

	std::vector<float32_t> array(4, -1.0F);
	transfer.read(
		array.data(), numerical_type::float32,
		as_file(file), numerical_type::float32,
		get_system_byte_order()
	);

	REQUIRE( transfer.get_region_count() == 0 );
	REQUIRE( array == std::vector<float32_t>{-1, -1, -1, -1} );
}
