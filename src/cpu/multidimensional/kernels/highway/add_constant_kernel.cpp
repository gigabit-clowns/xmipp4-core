// SPDX-License-Identifier: GPL-3.0-only

#undef HWY_TARGET_INCLUDE
#define HWY_TARGET_INCLUDE "cpu/multidimensional/kernels/highway/add_constant_kernel.cpp"

#include <hwy/foreach_target.h>
#include <hwy/highway.h>

#include "helpers/foreach_data_type.hpp"

#include <complex>

HWY_BEFORE_NAMESPACE();
namespace xmipp4 
{
namespace multidimensional
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
	const auto y = hn::OddEven(y_real, y_imag);
	AddConstantImpl(
		d, 
		reinterpret_cast<T*>(result), 
		reinterpret_cast<const T*>(x), 
		2*count, 
		y
	);
}



#if !HWY_HAVE_FLOAT16

void AddConstantImpl(
	hwy::float16_t* result, 
	const hwy::float16_t* x, 
	std::size_t count,
	const hwy::float16_t& value
) 
{
	const auto value_f32 = hwy::F32FromF16(value);

	// Very inefficient. Only as fallback
	for (std::size_t i = 0; i < count; ++i)
	{
		result[i] = 
			hwy::F16FromF32(hwy::F32FromF16(x[i]) + value_f32);
	}
}

void AddConstantImpl(
	std::complex<hwy::float16_t>* result, 
	const std::complex<hwy::float16_t>* x, 
	std::size_t count,
	const std::complex<hwy::float16_t>& value
) 
{
	const auto value_real_f32 = hwy::F32FromF16(value.real());
	const auto value_imag_f32 = hwy::F32FromF16(value.imag());

	// Very inefficient. Only as fallback
	for (std::size_t i = 0; i < count; ++i)
	{
		result[i] = std::complex<hwy::float16_t>(
			hwy::F16FromF32(hwy::F32FromF16(x[i].real()) + value_real_f32),
			hwy::F16FromF32(hwy::F32FromF16(x[i].imag()) + value_imag_f32)
		);
	}
}

#endif // !HWY_HAVE_FLOAT16



#define XMIPP4_HWY_DECLARE_ADD_CONSTANT(T, Suffix) \
	void AddConstant##Suffix( \
		T* result, \
		const T* x, \
		std::size_t count, \
		const T& value \
	) \
	{ \
		AddConstantImpl(result, x, count, value); \
	}

XMIPP4_HWY_FOR_EACH_ARITHMETIC_TYPE(XMIPP4_HWY_DECLARE_ADD_CONSTANT)

} // namespace HWY_NAMESPACE
} // namespace multidimensional
} // namespace xmipp4
HWY_AFTER_NAMESPACE();

#if HWY_ONCE

#include "add_constant_kernel.hpp"

#include <xmipp4/core/type_wrap.hpp>
#include <xmipp4/core/platform/assert.hpp>

namespace xmipp4 
{
namespace multidimensional
{

#define XMIPP4_HWY_EXPORT_ADD_CONSTANT(T, Suffix) \
	HWY_EXPORT(AddConstant##Suffix); \
	auto get_add_constant_kernel_pointer(type_tag<T>) \
	{ \
		return HWY_DYNAMIC_POINTER(AddConstant##Suffix); \
	}

XMIPP4_HWY_FOR_EACH_ARITHMETIC_TYPE(XMIPP4_HWY_EXPORT_ADD_CONSTANT)



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



#define XMIPP4_HWY_EXPORT_ADD_CONSTANT_KERNEL(T, Suffix) \
	template class add_constant_kernel<T>;

XMIPP4_HWY_FOR_EACH_ARITHMETIC_TYPE(XMIPP4_HWY_EXPORT_ADD_CONSTANT_KERNEL)

} // namespace multidimensional
} // namespace xmipp4

#endif  // HWY_ONCE
