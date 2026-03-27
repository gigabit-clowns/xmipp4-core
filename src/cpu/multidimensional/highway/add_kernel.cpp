// SPDX-License-Identifier: GPL-3.0-only

#undef HWY_TARGET_INCLUDE
#define HWY_TARGET_INCLUDE "cpu/multidimensional/highway/add_kernel.cpp"

#include "hwy/foreach_target.h"
#include "hwy/highway.h"

#include <complex>

HWY_BEFORE_NAMESPACE();
namespace xmipp4 
{
namespace HWY_NAMESPACE 
{

namespace hn = hwy::HWY_NAMESPACE;

#if !HWY_HAVE_FLOAT16

void AddImpl(
	hwy::float16_t* result, 
	const hwy::float16_t* x, 
	const hwy::float16_t* y, 
	std::size_t count
) 
{
	// Very inefficient. Only as fallback
	for (std::size_t i = 0; i < count; ++i)
	{
		result[i] = 
			hwy::F16FromF32(hwy::F32FromF16(x[i]) + hwy::F32FromF16(y[i]));
	}
}

#endif // !HWY_HAVE_FLOAT16

template <typename T>
void AddImpl(
	T* result_array, 
	const T* x_array, 
	const T* y_array, 
	std::size_t count
) 
{
	hn::ScalableTag<T> d;
	const auto n = hn::Lanes(d);

	std::size_t i = 0;
	if (count >= n) 
	{
 		for (; i <= count - n; i += n) {
			const auto x = hn::LoadU(d, x_array + i);
			const auto y = hn::LoadU(d, y_array + i);
			const auto result = hn::Add(x, y);
			hn::StoreU(result, d, result_array + i);
		}
	}

	if (i == count) return;
	const auto remaining = count - i;
	HWY_DASSERT(0 != remaining && remaining < n);
	const auto x = hn::LoadN(d, x_array + i, remaining);
	const auto y = hn::LoadN(d, y_array + i, remaining);
	const auto result = hn::Add(x, y);
	hn::StoreN(result, d, result_array + i, remaining);
}

template <typename T>
void AddImpl(
	std::complex<T>* result_array, 
	const std::complex<T>* x_array, 
	const std::complex<T>* y_array, 
	std::size_t count
) 
{
	AddImpl(
		reinterpret_cast<T*>(result_array),
		reinterpret_cast<const T*>(x_array),
		reinterpret_cast<const T*>(y_array),
		2*count
	);
}

void AddU8(
	std::uint8_t* result, 
	const std::uint8_t* x, 
	const std::uint8_t* y, 
	std::size_t count
)
{
	AddImpl(result, x, y, count);
}

void AddU16(
	std::uint16_t* result, 
	const std::uint16_t* x, 
	const std::uint16_t* y, 
	std::size_t count
)
{
	AddImpl(result, x, y, count);
}

void AddU32(
	std::uint32_t* result, 
	const std::uint32_t* x, 
	const std::uint32_t* y, 
	std::size_t count
)
{
	AddImpl(result, x, y, count);
}

void AddU64(
	std::uint64_t* result, 
	const std::uint64_t* x, 
	const std::uint64_t* y, 
	std::size_t count
)
{
	AddImpl(result, x, y, count);
}

void AddI8(
	std::int8_t* result, 
	const std::int8_t* x, 
	const std::int8_t* y, 
	std::size_t count
)
{
	AddImpl(result, x, y, count);
}

void AddI16(
	std::int16_t* result, 
	const std::int16_t* x, 
	const std::int16_t* y, 
	std::size_t count
)
{
	AddImpl(result, x, y, count);
}

void AddI32(
	std::int32_t* result, 
	const std::int32_t* x, 
	const std::int32_t* y, 
	std::size_t count
)
{
	AddImpl(result, x, y, count);
}

void AddI64(
	std::int64_t* result, 
	const std::int64_t* x, 
	const std::int64_t* y, 
	std::size_t count
)
{
	AddImpl(result, x, y, count);
}

void AddF16(
	hwy::float16_t* result, 
	const hwy::float16_t* x, 
	const hwy::float16_t* y, 
	std::size_t count
)
{
	AddImpl(result, x, y, count);
}

void AddF32(
	float* result, 
	const float* x, 
	const float* y, 
	std::size_t count
)
{
	AddImpl(result, x, y, count);
}

void AddF64(
	double* result, 
	const double* x, 
	const double* y, 
	std::size_t count
)
{
	AddImpl(result, x, y, count);
}

void AddC32(
	std::complex<hwy::float16_t>* result, 
	const std::complex<hwy::float16_t>* x, 
	const std::complex<hwy::float16_t>* y, 
	std::size_t count
)
{
	AddImpl(result, x, y, count);
}

void AddC64(
	std::complex<hwy::float32_t>* result, 
	const std::complex<hwy::float32_t>* x, 
	const std::complex<hwy::float32_t>* y, 
	std::size_t count
)
{
	AddImpl(result, x, y, count);
}

void AddC128(
	std::complex<hwy::float64_t>* result, 
	const std::complex<hwy::float64_t>* x, 
	const std::complex<hwy::float64_t>* y, 
	std::size_t count
)
{
	AddImpl(result, x, y, count);
}

}  // namespace HWY_NAMESPACE
}  // namespace xmipp4
HWY_AFTER_NAMESPACE();

#if HWY_ONCE

#include "add_kernel.hpp"

#include <xmipp4/core/type_wrap.hpp>
#include <xmipp4/core/platform/assert.hpp>

namespace xmipp4 
{

HWY_EXPORT(AddU8);
HWY_EXPORT(AddU16);
HWY_EXPORT(AddU32);
HWY_EXPORT(AddU64);
HWY_EXPORT(AddI8);
HWY_EXPORT(AddI16);
HWY_EXPORT(AddI32);
HWY_EXPORT(AddI64);
HWY_EXPORT(AddF32);
HWY_EXPORT(AddF64);
HWY_EXPORT(AddC64);
HWY_EXPORT(AddC128);

auto get_add_kernel_pointer(type_tag<std::uint8_t>)
{
	return HWY_DYNAMIC_POINTER(AddU8);
}

auto get_add_kernel_pointer(type_tag<std::uint16_t>)
{
	return HWY_DYNAMIC_POINTER(AddU16);
}

auto get_add_kernel_pointer(type_tag<std::uint32_t>)
{
	return HWY_DYNAMIC_POINTER(AddU32);
}

auto get_add_kernel_pointer(type_tag<std::uint64_t>)
{
	return HWY_DYNAMIC_POINTER(AddU64);
}

auto get_add_kernel_pointer(type_tag<std::int8_t>)
{
	return HWY_DYNAMIC_POINTER(AddI8);
}

auto get_add_kernel_pointer(type_tag<std::int16_t>)
{
	return HWY_DYNAMIC_POINTER(AddI16);
}

auto get_add_kernel_pointer(type_tag<std::int32_t>)
{
	return HWY_DYNAMIC_POINTER(AddI32);
}

auto get_add_kernel_pointer(type_tag<std::int64_t>)
{
	return HWY_DYNAMIC_POINTER(AddI64);
}

auto get_add_kernel_pointer(type_tag<float>)
{
	return HWY_DYNAMIC_POINTER(AddF32);
}

auto get_add_kernel_pointer(type_tag<double>)
{
	return HWY_DYNAMIC_POINTER(AddF64);
}

auto get_add_kernel_pointer(type_tag<std::complex<float>>)
{
	return HWY_DYNAMIC_POINTER(AddC64);
}

auto get_add_kernel_pointer(type_tag<std::complex<double>>)
{
	return HWY_DYNAMIC_POINTER(AddC128);
}

template <typename T>
auto get_add_kernel_pointer(type_tag<T>)
{
	return &HWY_STATIC_DISPATCH(AddImpl<T>); // Sub-optimal
}



template <typename T>
add_kernel<T>::add_kernel() noexcept
	: m_handle(get_add_kernel_pointer(type_tag<value_type>()))
{
}

template <typename T>
void add_kernel<T>::operator()(
	value_type* result, 
	const value_type* x, 
	const value_type* y, 
	std::size_t count
) const noexcept
{
	XMIPP4_ASSERT(m_handle);
	m_handle(result, x, y, count);
}

template class add_kernel<std::uint8_t>;
template class add_kernel<std::uint16_t>;
template class add_kernel<std::uint32_t>;
template class add_kernel<std::uint64_t>;
template class add_kernel<std::int8_t>;
template class add_kernel<std::int16_t>;
template class add_kernel<std::int32_t>;
template class add_kernel<std::int64_t>;
template class add_kernel<float>;
template class add_kernel<double>;
template class add_kernel<std::complex<float>>;
template class add_kernel<std::complex<double>>;

}  // namespace xmipp4

#endif  // HWY_ONCE
