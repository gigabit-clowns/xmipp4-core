// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/fixed_float.hpp>

#include <half.hpp>

namespace xmipp4
{

namespace
{

template <typename T>
float16_t convert_to_float16(T value)
{
	xmipp4::float16_t result;
	const half_float::half h(value);
	static_assert(sizeof(h) == sizeof(result), "Expected sizes to match");
	std::memcpy(&result, &h, sizeof(h));
	return result;
}

template <typename T>
T convert_from_float16(float16_t value)
{
	half_float::half h;
	static_assert(sizeof(h) == sizeof(value), "Expected sizes to match");
	std::memcpy(&h, &value, sizeof(h));
	return static_cast<T>(h);
}

} // anonymous namespace

float16_t::float16_t() noexcept
	: m_bits(0)
{
}

float16_t::float16_t(int value) noexcept
	: float16_t(convert_to_float16(value))
{
}

float16_t::float16_t(double value) noexcept
	: float16_t(convert_to_float16(value))
{
}

float16_t::float16_t(float value) noexcept
	: float16_t(convert_to_float16(value))
{
}

float16_t::operator float() const noexcept
{
	return convert_from_float16<float>(*this);
}

} // namespace xmipp4

