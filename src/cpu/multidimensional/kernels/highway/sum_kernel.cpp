// SPDX-License-Identifier: GPL-3.0-only

#undef HWY_TARGET_INCLUDE
#define HWY_TARGET_INCLUDE "cpu/multidimensional/kernels/highway/sum_kernel.cpp"

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
hn::VFromD<D> ParallelSum(
	D d, 
	const hn::TFromD<D>* values_array, 
	std::size_t count
) 
{
	const auto n = hn::Lanes(d);

	hn::VFromD<D> sum;
	std::size_t i = 0;
	if (count >= n) 
	{
		sum = hn::LoadU(d, values_array);
		i = n;
 		for (; i <= count - n; i += n) {
			const auto values = hn::LoadU(d, values_array + i);
			sum = hn::Add(sum, values);
		}
	}
	else
	{
		sum = hn::Zero(d);
	}

	const auto remaining = count - i;
	if (remaining > 0)
	{
		HWY_DASSERT(remaining < n);
		const auto values = hn::LoadN(d, values_array + i, remaining);
		sum = hn::Add(sum, values);
	}

	return sum;
}	

template <typename T>
T SumImpl(const T* values, std::size_t count) 
{
	hn::ScalableTag<T> d;
	return hn::ReduceSum(d, ParallelSum(d, values, count));
}

template <typename T>
std::complex<T> SumImpl(const std::complex<T>* values, std::size_t count) 
{
	hn::ScalableTag<T> d;
	const auto sum = 
		ParallelSum(d, reinterpret_cast<const T*>(values), 2*count);
	const auto zero = hn::Zero(d);
	const auto sum_real = hn::ReduceSum(d, hn::OddEven(zero, sum));
	const auto sum_imag = hn::ReduceSum(d, hn::OddEven(sum, zero));
	return std::complex<T>(sum_real, sum_imag);
}



#define XMIPP4_HWY_DECLARE_SUM(T, Suffix) \
	T Sum##Suffix( \
		const T* x, \
		std::size_t count \
	) \
	{ \
		return SumImpl(x, count); \
	}

XMIPP4_HWY_FOR_EACH_ARITHMETIC_TYPE(XMIPP4_HWY_DECLARE_SUM)

} // namespace HWY_NAMESPACE
} // namespace multidimensional
} // namespace xmipp4
HWY_AFTER_NAMESPACE();

#if HWY_ONCE

#include "sum_kernel.hpp"

#include <xmipp4/core/type_wrap.hpp>
#include <xmipp4/core/platform/assert.hpp>

namespace xmipp4 
{
namespace multidimensional
{

#define XMIPP4_HWY_EXPORT_SUM(T, Suffix) \
	HWY_EXPORT(Sum##Suffix); \
	auto get_sum_kernel_pointer(type_tag<T>) \
	{ \
		return HWY_DYNAMIC_POINTER(Sum##Suffix); \
	}

XMIPP4_HWY_FOR_EACH_ARITHMETIC_TYPE(XMIPP4_HWY_EXPORT_SUM)



template <typename T>
sum_kernel<T>::sum_kernel() noexcept
	: m_handle(get_sum_kernel_pointer(type_tag<value_type>()))
{
}

template <typename T>
typename sum_kernel<T>::value_type sum_kernel<T>::operator()(
	const value_type* values, 
	std::size_t count
) const noexcept
{
	XMIPP4_ASSERT(m_handle);
	return m_handle(values, count);
}

template class sum_kernel<std::uint8_t>;
template class sum_kernel<std::uint16_t>;
template class sum_kernel<std::uint32_t>;
template class sum_kernel<std::uint64_t>;
template class sum_kernel<std::int8_t>;
template class sum_kernel<std::int16_t>;
template class sum_kernel<std::int32_t>;
template class sum_kernel<std::int64_t>;
template class sum_kernel<float>;
template class sum_kernel<double>;
template class sum_kernel<std::complex<float>>;
template class sum_kernel<std::complex<double>>;

} // namespace multidimensional
} // namespace xmipp4

#endif  // HWY_ONCE
