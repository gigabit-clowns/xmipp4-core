// SPDX-License-Identifier: GPL-3.0-only

#undef HWY_TARGET_INCLUDE
#define HWY_TARGET_INCLUDE "cpu/multidimensional/kernels/highway/fill_constant_kernel.cpp"

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



#if !HWY_HAVE_FLOAT16

void FillConstantImpl(
	hwy::float16_t* result, 
	std::size_t count,
	const hwy::float16_t& value
) 
{
	FillConstantImpl(
		reinterpret_cast<std::uint16_t*>(result),
		count,
		reinterpret_cast<const std::uint16_t&>(value)
	);
}

void FillConstantImpl(
	std::complex<hwy::float16_t>* result, 
	std::size_t count,
	const std::complex<hwy::float16_t>& value
) 
{
	FillConstantImpl(
		reinterpret_cast<std::uint32_t*>(result),
		count,
		reinterpret_cast<const std::uint32_t&>(value)
	);
}

#endif // !HWY_HAVE_FLOAT16



#define XMIPP4_HWY_DECLARE_FILL_CONSTANT(T, Suffix) \
	void FillConstant##Suffix( \
		T* result, \
		std::size_t count, \
		const T& value \
	) \
	{ \
		FillConstantImpl(result, count, value); \
	}

XMIPP4_HWY_FOR_EACH_ARITHMETIC_TYPE(XMIPP4_HWY_DECLARE_FILL_CONSTANT)

} // namespace HWY_NAMESPACE
} // namespace multidimensional
} // namespace xmipp4
HWY_AFTER_NAMESPACE();

#if HWY_ONCE

#include "fill_constant_kernel.hpp"

#include <xmipp4/core/type_wrap.hpp>
#include <xmipp4/core/platform/assert.hpp>

namespace xmipp4 
{
namespace multidimensional
{

#define XMIPP4_HWY_EXPORT_FILL_CONSTANT(T, Suffix) \
	HWY_EXPORT(FillConstant##Suffix); \
	auto get_fill_constant_kernel_pointer(type_tag<T>) \
	{ \
		return HWY_DYNAMIC_POINTER(FillConstant##Suffix); \
	}

XMIPP4_HWY_FOR_EACH_ARITHMETIC_TYPE(XMIPP4_HWY_EXPORT_FILL_CONSTANT)



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



#define XMIPP4_HWY_EXPORT_FILL_CONSTANT_KERNEL(T, Suffix) \
	template class fill_constant_kernel<T>;

XMIPP4_HWY_FOR_EACH_ARITHMETIC_TYPE(XMIPP4_HWY_EXPORT_FILL_CONSTANT_KERNEL)

} // namespace multidimensional
} // namespace xmipp4

#endif  // HWY_ONCE
