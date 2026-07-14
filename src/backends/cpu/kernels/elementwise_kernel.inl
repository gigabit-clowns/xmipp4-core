// SPDX-License-Identifier: GPL-3.0-only

#include "elementwise_kernel.hpp"

#include <backends/cpu/loops/strided_pointer_iterator.hpp>

#include <initializer_list>
#include <tuple>
#include <utility>

namespace xmipp4
{
namespace cpu
{
namespace detail
{

template <bool...>
struct bool_pack
{
};

template <bool... Values>
using all_true =
	std::is_same<bool_pack<true, Values...>, bool_pack<Values..., true>>;

// True when every operand stride resolved to contiguous_stride_tag.
template <typename... Strides>
struct all_contiguous
{
	using type = typename all_true<
		std::is_same<Strides, contiguous_stride_tag>::value...
	>::type;
};

} // namespace detail

template <typename Op>
inline
elementwise_kernel<Op>::elementwise_kernel(Op op)
	: m_op(std::move(op))
{
}

template <typename Op>
template <typename... Pointers, typename... Strides>
inline
void elementwise_kernel<Op>::operator()(
	const std::tuple<Pointers*...> &pointers,
	const std::tuple<Strides...> &strides,
	std::size_t count
) const
{
	dispatch(
		pointers,
		strides,
		count,
		typename detail::all_contiguous<Strides...>::type()
	);
}

template <typename Op>
template <typename... Pointers, typename... Strides>
inline
void elementwise_kernel<Op>::dispatch(
	const std::tuple<Pointers*...> &pointers,
	const std::tuple<Strides...> &/*strides*/,
	std::size_t count,
	std::true_type
) const
{
	contiguous_loop(pointers, count, std::index_sequence_for<Pointers...>());
}

template <typename Op>
template <typename... Pointers, typename... Strides>
inline
void elementwise_kernel<Op>::dispatch(
	const std::tuple<Pointers*...> &pointers,
	const std::tuple<Strides...> &strides,
	std::size_t count,
	std::false_type
) const
{
	generic_loop(
		pointers,
		strides,
		count,
		std::index_sequence_for<Pointers...>()
	);
}

template <typename Op>
template <typename... Pointers, std::size_t... Is>
inline
void elementwise_kernel<Op>::contiguous_loop(
	const std::tuple<Pointers*...> &pointers,
	std::size_t count,
	std::index_sequence<Is...>
) const
{
	for (std::size_t i = 0; i < count; ++i)
	{
		m_op((std::get<Is>(pointers) + i)...);
	}
}

template <typename Op>
template <typename... Pointers, typename... Strides, std::size_t... Is>
inline
void elementwise_kernel<Op>::generic_loop(
	const std::tuple<Pointers*...> &pointers,
	const std::tuple<Strides...> &strides,
	std::size_t count,
	std::index_sequence<Is...>
) const
{
	auto iterators = std::make_tuple(
		make_strided_pointer_iterator(
			std::get<Is>(pointers),
			std::get<Is>(strides)
		)...
	);
	for (std::size_t i = 0; i < count; ++i)
	{
		m_op(std::get<Is>(iterators).data()...);
		(void) std::initializer_list<int> {
			(++std::get<Is>(iterators), 0)...
		};
	}
}

template <typename Op>
inline
elementwise_kernel<Op> make_elementwise_kernel(Op op)
{
	return elementwise_kernel<Op>(std::move(op));
}

} // namespace cpu
} // namespace xmipp4
