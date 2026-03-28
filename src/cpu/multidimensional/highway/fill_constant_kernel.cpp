// SPDX-License-Identifier: GPL-3.0-only

#undef HWY_TARGET_INCLUDE
#define HWY_TARGET_INCLUDE "cpu/multidimensional/highway/fill_constant_kernel.cpp"

#include "hwy/foreach_target.h"
#include "hwy/highway.h"

#include <complex>

HWY_BEFORE_NAMESPACE();
namespace xmipp4 
{
namespace HWY_NAMESPACE 
{

namespace hn = hwy::HWY_NAMESPACE;

template <typename D>
void FillConstantImpl(
	D d,
	hn::TFromD<D>* result_array, 
	std::size_t count,
	const hn::VFromD<D>& y
) 
{
	const auto n = hn::Lanes(d);

	std::size_t i = 0;
	if (count >= n) 
	{
 		for (; i <= count - n; i += n) {
			hn::StoreU(y, d, result_array + i);
		}
	}

	if (i == count) return;
	const auto remaining = count - i;
	HWY_DASSERT(0 != remaining && remaining < n);
	hn::StoreN(y, d, result_array + i, remaining);
}

template <typename T>
void FillConstantImpl(
	T* result, 
	std::size_t count,
	const T& value
) 
{
	hn::ScalableTag<T> d;
	const auto y = hn::Set(d, value);
	FillConstantImpl(d, result, count, y);
}

template <typename T>
void FillConstantImpl(
	std::complex<T>* result, 
	std::size_t count,
	const std::complex<T> &value
) 
{
	hn::ScalableTag<T> d;
	const auto y_real = hn::Set(d, value.real());
	const auto y_imag = hn::Set(d, value.imag());
	const auto y = hn::OddEven(y_imag, y_real);
	FillConstantImpl(
		d, 
		reinterpret_cast<T*>(result), 
		2*count, 
		y
	);
}

void FillConstantU8(
	std::uint8_t* result, 
	std::size_t count,
	const std::uint8_t& value
)
{
	FillConstantImpl(result, count, value);
}

void FillConstantU16(
	std::uint16_t* result, 
	std::size_t count,
	const std::uint16_t& value
)
{
	FillConstantImpl(result, count, value);
}

void FillConstantU32(
	std::uint32_t* result, 
	std::size_t count,
	const std::uint32_t& value
)
{
	FillConstantImpl(result, count, value);
}

void FillConstantU64(
	std::uint64_t* result, 
	std::size_t count,
	const std::uint64_t& value
)
{
	FillConstantImpl(result, count, value);
}

void FillConstantI8(
	std::int8_t* result, 
	std::size_t count,
	const std::int8_t& value
)
{
	FillConstantImpl(result, count, value);
}

void FillConstantI16(
	std::int16_t* result, 
	std::size_t count,
	const std::int16_t& value
)
{
	FillConstantImpl(result, count, value);
}

void FillConstantI32(
	std::int32_t* result, 
	std::size_t count,
	const std::int32_t& value
)
{
	FillConstantImpl(result, count, value);
}

void FillConstantI64(
	std::int64_t* result, 
	std::size_t count,
	const std::int64_t& value
)
{
	FillConstantImpl(result, count, value);
}

void FillConstantF32(
	float* result, 
	std::size_t count,
	const float& value
)
{
	FillConstantImpl(result, count, value);
}

void FillConstantF64(
	double* result, 
	std::size_t count,
	const double &value
)
{
	FillConstantImpl(result, count, value);
}

void FillConstantC64(
	std::complex<float>* result, 
	std::size_t count,
	const std::complex<float> &value
)
{
	FillConstantImpl(result, count, value);
}

void FillConstantC128(
	std::complex<double>* result, 
	std::size_t count,
	const std::complex<double> &value
)
{
	FillConstantImpl(result, count, value);
}

}  // namespace HWY_NAMESPACE
}  // namespace xmipp4
HWY_AFTER_NAMESPACE();

#if HWY_ONCE

#include "fill_constant_kernel.hpp"

#include <xmipp4/core/type_wrap.hpp>
#include <xmipp4/core/platform/assert.hpp>

namespace xmipp4 
{

HWY_EXPORT(FillConstantU8);
HWY_EXPORT(FillConstantU16);
HWY_EXPORT(FillConstantU32);
HWY_EXPORT(FillConstantU64);
HWY_EXPORT(FillConstantI8);
HWY_EXPORT(FillConstantI16);
HWY_EXPORT(FillConstantI32);
HWY_EXPORT(FillConstantI64);
HWY_EXPORT(FillConstantF32);
HWY_EXPORT(FillConstantF64);
HWY_EXPORT(FillConstantC64);
HWY_EXPORT(FillConstantC128);

auto get_fill_constant_kernel_pointer(type_tag<std::uint8_t>)
{
	return HWY_DYNAMIC_POINTER(FillConstantU8);
}

auto get_fill_constant_kernel_pointer(type_tag<std::uint16_t>)
{
	return HWY_DYNAMIC_POINTER(FillConstantU16);
}

auto get_fill_constant_kernel_pointer(type_tag<std::uint32_t>)
{
	return HWY_DYNAMIC_POINTER(FillConstantU32);
}

auto get_fill_constant_kernel_pointer(type_tag<std::uint64_t>)
{
	return HWY_DYNAMIC_POINTER(FillConstantU64);
}

auto get_fill_constant_kernel_pointer(type_tag<std::int8_t>)
{
	return HWY_DYNAMIC_POINTER(FillConstantI8);
}

auto get_fill_constant_kernel_pointer(type_tag<std::int16_t>)
{
	return HWY_DYNAMIC_POINTER(FillConstantI16);
}

auto get_fill_constant_kernel_pointer(type_tag<std::int32_t>)
{
	return HWY_DYNAMIC_POINTER(FillConstantI32);
}

auto get_fill_constant_kernel_pointer(type_tag<std::int64_t>)
{
	return HWY_DYNAMIC_POINTER(FillConstantI64);
}

auto get_fill_constant_kernel_pointer(type_tag<float>)
{
	return HWY_DYNAMIC_POINTER(FillConstantF32);
}

auto get_fill_constant_kernel_pointer(type_tag<double>)
{
	return HWY_DYNAMIC_POINTER(FillConstantF64);
}

auto get_fill_constant_kernel_pointer(type_tag<std::complex<float>>)
{
	return HWY_DYNAMIC_POINTER(FillConstantC64);
}

auto get_fill_constant_kernel_pointer(type_tag<std::complex<double>>)
{
	return HWY_DYNAMIC_POINTER(FillConstantC128);
}

template <typename T>
auto get_fill_constant_kernel_pointer(type_tag<T>)
{
	return &HWY_STATIC_DISPATCH(FillConstantImpl<T>); // Sub-optimal
}



template <typename T>
fill_constant_kernel<T>::fill_constant_kernel() noexcept
	: m_handle(get_fill_constant_kernel_pointer(type_tag<value_type>()))
{
}

template <typename T>
void fill_constant_kernel<T>::operator()(
	value_type* result, 
	std::size_t count,
	const value_type& value
) const noexcept
{
	XMIPP4_ASSERT(m_handle);
	m_handle(result, count, value);
}

template class fill_constant_kernel<std::uint8_t>;
template class fill_constant_kernel<std::uint16_t>;
template class fill_constant_kernel<std::uint32_t>;
template class fill_constant_kernel<std::uint64_t>;
template class fill_constant_kernel<std::int8_t>;
template class fill_constant_kernel<std::int16_t>;
template class fill_constant_kernel<std::int32_t>;
template class fill_constant_kernel<std::int64_t>;
template class fill_constant_kernel<float>;
template class fill_constant_kernel<double>;
template class fill_constant_kernel<std::complex<float>>;
template class fill_constant_kernel<std::complex<double>>;

}  // namespace xmipp4

#endif  // HWY_ONCE
