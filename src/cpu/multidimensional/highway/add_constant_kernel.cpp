// SPDX-License-Identifier: GPL-3.0-only

#undef HWY_TARGET_INCLUDE
#define HWY_TARGET_INCLUDE "cpu/multidimensional/highway/add_constant_kernel.cpp"

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
void AddConstantImpl(
	D d,
	hn::TFromD<D>* result_array, 
	const hn::TFromD<D>* x_array, 
	std::size_t count,
	const hn::VFromD<D>& y
) 
{
	const auto n = hn::Lanes(d);

	std::size_t i = 0;
	if (count >= n) 
	{
 		for (; i <= count - n; i += n) {
			const auto x = hn::LoadU(d, x_array + i);
			const auto result = hn::Add(x, y);
			hn::StoreU(result, d, result_array + i);
		}
	}

	if (i == count) return;
	const auto remaining = count - i;
	HWY_DASSERT(0 != remaining && remaining < n);
	const auto x = hn::LoadN(d, x_array + i, remaining);
	const auto result = hn::Add(x, y);
	hn::StoreN(result, d, result_array + i, remaining);
}

template <typename T>
void AddConstantImpl(
	T* result, 
	const T* x, 
	std::size_t count,
	const T& value
) 
{
	hn::ScalableTag<T> d;
	const auto y = hn::Set(d, value);
	AddConstantImpl(d, result, x, count, y);
}

template <typename T>
void AddConstantImpl(
	std::complex<T>* result, 
	const std::complex<T>* x, 
	std::size_t count,
	const std::complex<T> &value
) 
{
	hn::ScalableTag<T> d;
	const auto y_real = hn::Set(d, value.real());
	const auto y_imag = hn::Set(d, value.imag());
	const auto y = hn::InterleaveLower(y_real, y_imag);
	AddConstantImpl(
		d, 
		reinterpret_cast<T*>(result), 
		reinterpret_cast<const T*>(x), 
		2*count, 
		y
	);
}

void AddConstantU16(
	std::uint16_t* result, 
	const std::uint16_t* x, 
	std::size_t count,
	const std::uint16_t& value
)
{
	AddConstantImpl(result, x, count, value);
}

void AddConstantU32(
	std::uint32_t* result, 
	const std::uint32_t* x, 
	std::size_t count,
	const std::uint32_t& value
)
{
	AddConstantImpl(result, x, count, value);
}

void AddConstantU64(
	std::uint64_t* result, 
	const std::uint64_t* x, 
	std::size_t count,
	const std::uint64_t& value
)
{
	AddConstantImpl(result, x, count, value);
}

void AddConstantI16(
	std::int16_t* result, 
	const std::int16_t* x, 
	std::size_t count,
	const std::int16_t& value
)
{
	AddConstantImpl(result, x, count, value);
}

void AddConstantI32(
	std::int32_t* result, 
	const std::int32_t* x, 
	std::size_t count,
	const std::int32_t& value
)
{
	AddConstantImpl(result, x, count, value);
}

void AddConstantI64(
	std::int64_t* result, 
	const std::int64_t* x, 
	std::size_t count,
	const std::int64_t& value
)
{
	AddConstantImpl(result, x, count, value);
}

void AddConstantF32(
	float* result, 
	const float* x, 
	std::size_t count,
	const float& value
)
{
	AddConstantImpl(result, x, count, value);
}

void AddConstantF64(
	double* result, 
	const double* x, 
	std::size_t count,
	const double &value
)
{
	AddConstantImpl(result, x, count, value);
}

void AddConstantC64(
	std::complex<float>* result, 
	const std::complex<float>* x, 
	std::size_t count,
	const std::complex<float> &value
)
{
	AddConstantImpl(result, x, count, value);
}

void AddConstantC128(
	std::complex<double>* result, 
	const std::complex<double>* x, 
	std::size_t count,
	const std::complex<double> &value
)
{
	AddConstantImpl(result, x, count, value);
}

}  // namespace HWY_NAMESPACE
}  // namespace xmipp4
HWY_AFTER_NAMESPACE();

#if HWY_ONCE

#include "add_constant_kernel.hpp"

#include <xmipp4/core/type_wrap.hpp>
#include <xmipp4/core/platform/assert.hpp>

namespace xmipp4 
{

HWY_EXPORT(AddConstantU16);
HWY_EXPORT(AddConstantU32);
HWY_EXPORT(AddConstantU64);
HWY_EXPORT(AddConstantI16);
HWY_EXPORT(AddConstantI32);
HWY_EXPORT(AddConstantI64);
HWY_EXPORT(AddConstantF32);
HWY_EXPORT(AddConstantF64);
HWY_EXPORT(AddConstantC64);
HWY_EXPORT(AddConstantC128);

auto get_add_constant_kernel_pointer(type_tag<std::uint16_t>)
{
	return HWY_DYNAMIC_POINTER(AddConstantU16);
}

auto get_add_constant_kernel_pointer(type_tag<std::uint32_t>)
{
	return HWY_DYNAMIC_POINTER(AddConstantU32);
}

auto get_add_constant_kernel_pointer(type_tag<std::uint64_t>)
{
	return HWY_DYNAMIC_POINTER(AddConstantU64);
}

auto get_add_constant_kernel_pointer(type_tag<std::int16_t>)
{
	return HWY_DYNAMIC_POINTER(AddConstantI16);
}

auto get_add_constant_kernel_pointer(type_tag<std::int32_t>)
{
	return HWY_DYNAMIC_POINTER(AddConstantI32);
}

auto get_add_constant_kernel_pointer(type_tag<std::int64_t>)
{
	return HWY_DYNAMIC_POINTER(AddConstantI64);
}

auto get_add_constant_kernel_pointer(type_tag<float>)
{
	return HWY_DYNAMIC_POINTER(AddConstantF32);
}

auto get_add_constant_kernel_pointer(type_tag<double>)
{
	return HWY_DYNAMIC_POINTER(AddConstantF64);
}

auto get_add_constant_kernel_pointer(type_tag<std::complex<float>>)
{
	return HWY_DYNAMIC_POINTER(AddConstantC64);
}

auto get_add_constant_kernel_pointer(type_tag<std::complex<double>>)
{
	return HWY_DYNAMIC_POINTER(AddConstantC128);
}

template <typename T>
auto get_add_constant_kernel_pointer(type_tag<T>)
{
	return &HWY_STATIC_DISPATCH(AddConstantImpl<T>); // Sub-optimal
}



template <typename T>
add_constant_kernel<T>::add_constant_kernel() noexcept
	: m_handle(get_add_constant_kernel_pointer(type_tag<value_type>()))
{
}

template <typename T>
void add_constant_kernel<T>::operator()(
	value_type* result, 
	const value_type* x, 
	std::size_t count,
	const value_type& value
) const noexcept
{
	XMIPP4_ASSERT(m_handle);
	m_handle(result, x, count, value);
}

template class add_constant_kernel<std::uint16_t>;
template class add_constant_kernel<std::uint32_t>;
template class add_constant_kernel<std::uint64_t>;
template class add_constant_kernel<std::int16_t>;
template class add_constant_kernel<std::int32_t>;
template class add_constant_kernel<std::int64_t>;
template class add_constant_kernel<float>;
template class add_constant_kernel<double>;
template class add_constant_kernel<std::complex<float>>;
template class add_constant_kernel<std::complex<double>>;

}  // namespace xmipp4

#endif  // HWY_ONCE
