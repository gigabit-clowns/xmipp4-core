// SPDX-License-Identifier: GPL-3.0-only

#undef HWY_TARGET_INCLUDE
#define HWY_TARGET_INCLUDE "cpu/multidimensional/highway/sum_kernel.cpp"

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

std::uint16_t SumU16(
	const std::uint16_t* values, 
	std::size_t count
)
{
	return SumImpl(values, count);
}

std::uint32_t SumU32(
	const std::uint32_t* values, 
	std::size_t count
)
{
	return SumImpl(values, count);
}

std::uint64_t SumU64(
	const std::uint64_t* values, 
	std::size_t count
)
{
	return SumImpl(values, count);
}

std::int16_t SumI16(
	const std::int16_t* values, 
	std::size_t count
)
{
	return SumImpl(values, count);
}

std::int32_t SumI32(
	const std::int32_t* values, 
	std::size_t count
)
{
	return SumImpl(values, count);
}

std::int64_t SumI64(
	const std::int64_t* values, 
	std::size_t count
)
{
	return SumImpl(values, count);
}

float SumF32(
	const float* values, 
	std::size_t count
)
{
	return SumImpl(values, count);
}

double SumF64(
	const double* values, 
	std::size_t count
)
{
	return SumImpl(values, count);
}

std::complex<float> SumC64(
	const std::complex<float>* values, 
	std::size_t count
)
{
	return SumImpl(values, count);
}

std::complex<double> SumC128(
	const std::complex<double>* values, 
	std::size_t count
)
{
	return SumImpl(values, count);
}

}  // namespace HWY_NAMESPACE
}  // namespace xmipp4
HWY_AFTER_NAMESPACE();

#if HWY_ONCE

#include "sum_kernel.hpp"

#include <xmipp4/core/type_wrap.hpp>
#include <xmipp4/core/platform/assert.hpp>

namespace xmipp4 
{

HWY_EXPORT(SumU16);
HWY_EXPORT(SumU32);
HWY_EXPORT(SumU64);
HWY_EXPORT(SumI16);
HWY_EXPORT(SumI32);
HWY_EXPORT(SumI64);
HWY_EXPORT(SumF32);
HWY_EXPORT(SumF64);
HWY_EXPORT(SumC64);
HWY_EXPORT(SumC128);

auto get_sum_kernel_pointer(type_tag<std::uint16_t>)
{
	return HWY_DYNAMIC_POINTER(SumU16);
}

auto get_sum_kernel_pointer(type_tag<std::uint32_t>)
{
	return HWY_DYNAMIC_POINTER(SumU32);
}

auto get_sum_kernel_pointer(type_tag<std::uint64_t>)
{
	return HWY_DYNAMIC_POINTER(SumU64);
}

auto get_sum_kernel_pointer(type_tag<std::int16_t>)
{
	return HWY_DYNAMIC_POINTER(SumI16);
}

auto get_sum_kernel_pointer(type_tag<std::int32_t>)
{
	return HWY_DYNAMIC_POINTER(SumI32);
}

auto get_sum_kernel_pointer(type_tag<std::int64_t>)
{
	return HWY_DYNAMIC_POINTER(SumI64);
}

auto get_sum_kernel_pointer(type_tag<float>)
{
	return HWY_DYNAMIC_POINTER(SumF32);
}

auto get_sum_kernel_pointer(type_tag<double>)
{
	return HWY_DYNAMIC_POINTER(SumF64);
}

auto get_sum_kernel_pointer(type_tag<std::complex<float>>)
{
	return HWY_DYNAMIC_POINTER(SumC64);
}

auto get_sum_kernel_pointer(type_tag<std::complex<double>>)
{
	return HWY_DYNAMIC_POINTER(SumC128);
}

template <typename T>
auto get_sum_kernel_pointer(type_tag<T>)
{
	return &HWY_STATIC_DISPATCH(SumImpl<T>); // Sub-optimal
}



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

template class sum_kernel<std::uint16_t>;
template class sum_kernel<std::uint32_t>;
template class sum_kernel<std::uint64_t>;
template class sum_kernel<std::int16_t>;
template class sum_kernel<std::int32_t>;
template class sum_kernel<std::int64_t>;
template class sum_kernel<float>;
template class sum_kernel<double>;
template class sum_kernel<std::complex<float>>;
template class sum_kernel<std::complex<double>>;

}  // namespace xmipp4

#endif  // HWY_ONCE
