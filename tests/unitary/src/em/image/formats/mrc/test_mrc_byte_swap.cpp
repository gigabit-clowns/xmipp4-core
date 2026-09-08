// SPDX-License-Identifier: GPL-3.0-only

#include <catch2/catch_test_macros.hpp>

#include <em/image/formats/mrc/mrc_byte_swap.hpp>

#include <rexlib/core/numerical/fixed_width_float.hpp>

#include <algorithm>
#include <complex>
#include <cstdint>
#include <cstring>
#include <vector>

using namespace rexlib;
using namespace rexlib::em;
using namespace rexlib::em::mrc;

namespace
{

// Reverses the bytes of a value without going through the helper under test,
// so that the two are compared rather than one restating the other.
template <typename T>
T reversed(const T &value)
{
	unsigned char raw[sizeof(T)];
	std::memcpy(raw, &value, sizeof(raw));
	std::reverse(raw, raw + sizeof(raw));

	T result;
	std::memcpy(&result, raw, sizeof(result));
	return result;
}

template <typename T>
std::vector<unsigned char> bytes_of(const T &value)
{
	std::vector<unsigned char> raw(sizeof(T));
	std::memcpy(raw.data(), &value, raw.size());
	return raw;
}

} // anonymous namespace

TEST_CASE( "the bytes of a whole element are reversed", "[mrc_byte_swap]" )
{
	SECTION( "a two byte integer is reversed" )
	{
		REQUIRE( byte_swap<std::uint16_t>(0x1234) == 0x3412 );
		REQUIRE( byte_swap<std::int16_t>(0x0102) == 0x0201 );
	}

	SECTION( "a four byte float is reversed through its bits" )
	{
		const float value = 1.5F;

		REQUIRE( bytes_of(byte_swap(value)) == bytes_of(reversed(value)) );
	}

	SECTION( "half precision is reversed through its bits" )
	{
		const float16_t value(1.5F);
		const auto swapped = byte_swap(value);

		REQUIRE( swapped.get_bits() ==
			static_cast<std::uint16_t>(
				(value.get_bits() << 8) | (value.get_bits() >> 8)) );
	}

	SECTION( "reversing twice is the identity" )
	{
		const std::uint16_t integer = 0xABCD;
		const float real = -2.25F;

		REQUIRE( byte_swap(byte_swap(integer)) == integer );
		REQUIRE( byte_swap(byte_swap(real)) == real );
	}
}

TEST_CASE( "a single byte is the same in either order", "[mrc_byte_swap]" )
{
	// Mode 0 is one byte wide, so the swapping path has to be a no-op for it
	// rather than something to be skipped by the caller.
	REQUIRE( byte_swap<std::int8_t>(-2) == -2 );
	REQUIRE( byte_swap<std::uint8_t>(0xF0) == 0xF0 );
}

TEST_CASE( "a complex element is reversed one component at a time",
	"[mrc_byte_swap]" )
{
	const std::complex<float> value(1.5F, -2.25F);
	const auto swapped = byte_swap(value);

	SECTION( "each component is reversed on its own" )
	{
		REQUIRE( bytes_of(swapped.real()) ==
			bytes_of(reversed(value.real())) );
		REQUIRE( bytes_of(swapped.imag()) ==
			bytes_of(reversed(value.imag())) );
	}

	SECTION( "it is not the reversal of the whole eight bytes" )
	{
		// The trap this guards: a complex value is two numbers, not one
		// twice as wide, so reversing all of its bytes at once would also
		// exchange its components.
		REQUIRE( bytes_of(swapped) != bytes_of(reversed(value)) );
	}

	SECTION( "reversing twice is the identity" )
	{
		REQUIRE( byte_swap(swapped) == value );
	}
}
