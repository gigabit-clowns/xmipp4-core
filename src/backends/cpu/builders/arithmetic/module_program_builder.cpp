// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/ops/arithmetic/modulo_operation.hpp>

#include <backends/cpu/builders/elementwise_program_builder.hpp>
#include <backends/cpu/builders/default_kernel_factory.hpp>
#include <backends/cpu/builders/type_dispatchers/homogeneous_type_dispatcher.hpp>
#include <backends/cpu/load_store.hpp>

#include <cmath>

namespace xmipp4
{
namespace cpu
{

namespace
{

template <typename T>
T to_divisor_sign(T remainder, T divisor)
{
	return (remainder != T(0) && (remainder < T(0)) != (divisor < T(0))) ?
		remainder + divisor : remainder;
}

template <typename T>
typename std::enable_if<
	std::is_integral<T>::value && std::is_unsigned<T>::value,
	T
>::type
floor_mod(T x, T y)
{
	return x % y;
}

template <typename T>
typename std::enable_if<
	std::is_integral<T>::value && std::is_signed<T>::value,
	T
>::type
floor_mod(T x, T y)
{
	return to_divisor_sign(static_cast<T>(x % y), y);
}

template <typename T>
typename std::enable_if<!std::is_integral<T>::value, T>::type
floor_mod(T x, T y)
{
	using std::fmod;
	return to_divisor_sign(fmod(x, y), y);
}

struct modulo_kernel
{
	template <typename T>
	void operator()(T *result, const T *x, const T *y) const noexcept
	{
		store(result, floor_mod(load(x), load(y)));
	}
};

template <typename T>
struct modulo_predicate : std::integral_constant<
	bool,
	(std::is_integral<T>::value && !std::is_same<T, bool>::value) ||
	xmipp4::is_floating_point<T>::value
> {};

} // anonymous namespace

XMIPP4_REGISTER_ELEMENTWISE_PROGRAM_BUILDER(
	modulo,
	ops::modulo_operation,
	default_kernel_factory<modulo_kernel>,
	homogeneous_type_dispatcher<modulo_predicate>
);

} // namespace cpu
} // namespace xmipp4
