// SPDX-License-Identifier: GPL-3.0-only

#undef HWY_TARGET_INCLUDE
#define HWY_TARGET_INCLUDE "cpu/multidimensional/kernels/highway/add_kernel.cpp"

#include <hwy/foreach_target.h>
#include <hwy/highway.h>

#include "helpers/foreach_data_type.hpp"
#include "helpers/dispatch_pointer.hpp"

#include <complex>

HWY_BEFORE_NAMESPACE();
namespace xmipp4 
{
namespace multidimensional
{
namespace HWY_NAMESPACE 
{

namespace hn = hwy::HWY_NAMESPACE;

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



#define XMIPP4_HWY_DECLARE_ADD(T, Suffix) \
	void Add##Suffix( \
		T* result, \
		const T* x, \
		const T* y, \
		std::size_t count \
	) \
	{ \
		AddImpl(result, x, y, count); \
	}

XMIPP4_HWY_FOR_EACH_ARITHMETIC_TYPE(XMIPP4_HWY_DECLARE_ADD)

} // namespace HWY_NAMESPACE
} // namespace multidimensional
} // namespace xmipp4
HWY_AFTER_NAMESPACE();

#if HWY_ONCE

#include "add_kernel.hpp"

#include <xmipp4/core/type_wrap.hpp>
#include <xmipp4/core/platform/assert.hpp>

namespace xmipp4 
{
namespace multidimensional
{

#define XMIPP4_HWY_EXPORT_ADD(T, Suffix) \
	XMIPP4_HWY_EXPORT(Add##Suffix); \
	auto get_add_kernel_pointer(type_tag<T>) \
	{ \
		return XMIPP4_HWY_FUNCTION_POINTER(Add##Suffix); \
	}

XMIPP4_HWY_FOR_EACH_ARITHMETIC_TYPE(XMIPP4_HWY_EXPORT_ADD)



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



#define XMIPP4_HWY_EXPORT_ADD_KERNEL(T, Suffix) \
	template class add_kernel<T>;

XMIPP4_HWY_FOR_EACH_ARITHMETIC_TYPE(XMIPP4_HWY_EXPORT_ADD_KERNEL)

} // namespace multidimensional
} // namespace xmipp4

#endif  // HWY_ONCE
