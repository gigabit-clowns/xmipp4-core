// SPDX-License-Identifier: GPL-3.0-only

#include "elementwise_loop.hpp"

#include "inner_loop_stride_dispatch.hpp"
#include "strided_pointer_iterator.hpp"

#include <xmipp4/core/layout/joint_cursor.hpp>

#include <initializer_list>
#include <tuple>
#include <utility>
#include <type_traits>

namespace xmipp4
{
namespace cpu
{
namespace detail
{

template <typename InnerLoop, typename... Pointers, std::size_t... Is>
inline
void run_elementwise_outer_loop_impl(
	InnerLoop &&inner_loop,
	const joint_layout &layout,
	const std::tuple<Pointers...> &pointers,
	std::index_sequence<Is...>
)
{
	joint_cursor ite;
	std::size_t count = layout.iter(ite);
	if (count == 0)
	{
		return;
	}

	const auto offsets = ite.get_offsets(); // Stable address
	do
	{
		inner_loop(std::get<Is>(pointers) + offsets[Is]..., count);
	}
	while ((count = layout.next(ite, count)));
}

template <bool...>
struct bool_pack
{
};

template <bool... Values>
using all_true =
	std::is_same<bool_pack<true, Values...>, bool_pack<Values..., true>>;

template <typename... Strides>
struct all_contiguous
{
	using type = typename all_true<
		std::is_same<Strides, contiguous_stride_tag>::value...
	>::type;
};

template <typename Op, typename... Pointers, std::size_t... Is>
inline
void run_contiguous_element_loop(
	const Op &op,
	const std::tuple<Pointers*...> &pointers,
	std::size_t count,
	std::index_sequence<Is...>
)
{
	static_assert(
		sizeof...(Is) == sizeof...(Pointers), 
		"Index count and pointer count must match."
	);

	for (std::size_t i = 0; i < count; ++i)
	{
		op((std::get<Is>(pointers) + i)...);
	}
}

template <
	typename Op,
	typename... Pointers,
	typename... Strides,
	std::size_t... Is
>
inline
void run_generic_element_loop(
	const Op &op,
	const std::tuple<Pointers*...> &pointers,
	const std::tuple<Strides...> &strides,
	std::size_t count,
	std::index_sequence<Is...>
)
{
	static_assert(
		sizeof...(Is) == sizeof...(Pointers), 
		"Index count and pointer count must match,"
	);
	static_assert(
		sizeof...(Is) == sizeof...(Strides), 
		"Index count and stride count must match."
	);

	auto iterators = std::make_tuple(
		make_strided_pointer_iterator(
			std::get<Is>(pointers),
			std::get<Is>(strides)
		)...
	);
	for (std::size_t i = 0; i < count; ++i)
	{
		op(std::get<Is>(iterators).data()...);
		(void) std::initializer_list<int> {
			(++std::get<Is>(iterators), 0)...
		};
	}
}

template <typename Op, typename... Pointers, typename... Strides>
inline
void dispatch_element_loop(
	const Op &op,
	const std::tuple<Pointers*...> &pointers,
	const std::tuple<Strides...> &/*strides*/,
	std::size_t count,
	std::true_type
)
{
	run_contiguous_element_loop(
		op, 
		pointers, 
		count, 
		std::index_sequence_for<Pointers...>()
	);
}

template <typename Op, typename... Pointers, typename... Strides>
inline
void dispatch_element_loop(
	const Op &op,
	const std::tuple<Pointers*...> &pointers,
	const std::tuple<Strides...> &strides,
	std::size_t count,
	std::false_type
)
{
	run_generic_element_loop(
		op, 
		pointers, 
		strides, 
		count, 
		std::index_sequence_for<Pointers...>()
	);
}

template <typename Op, typename... Pointers, typename... Strides>
inline
void run_element_loop(
	const Op &op,
	const std::tuple<Pointers*...> &pointers,
	const std::tuple<Strides...> &strides,
	std::size_t count
)
{
	dispatch_element_loop(
		op,
		pointers,
		strides,
		count,
		typename all_contiguous<Strides...>::type()
	);
}

} // namespace detail

template <typename InnerLoop, typename... Pointers>
inline
void run_elementwise_outer_loop(
	InnerLoop &&inner_loop,
	const joint_layout &layout,
	Pointers... pointers
)
{
	detail::run_elementwise_outer_loop_impl(
		std::forward<InnerLoop>(inner_loop),
		layout,
		std::make_tuple(pointers...),
		std::make_index_sequence<sizeof...(Pointers)>()
	);
}

template <typename Kernel, typename... Pointers>
inline
void run_elementwise_vector_loop(
	const Kernel &kernel,
	const joint_layout &layout,
	Pointers... pointers
)
{
	dispatch_inner_loop_strides(
		[&kernel, &layout, &pointers...] (auto strides)
		{
			run_elementwise_outer_loop(
				[&kernel, &strides]
				(Pointers... vector_pointers, std::size_t count)
				{
					kernel(
						std::make_tuple(vector_pointers...),
						strides,
						count
					);
				},
				layout,
				pointers...
			);
		},
		layout,
		std::integral_constant<std::size_t, sizeof...(Pointers)>()
	);
}

template <typename Op, typename... Pointers>
inline
void run_elementwise_loop(
	const Op &op,
	const joint_layout &layout,
	Pointers... pointers
)
{
	run_elementwise_vector_loop(
		[&op] (const auto &pointers, const auto &strides, std::size_t count)
		{
			detail::run_element_loop(op, pointers, strides, count);
		},
		layout,
		pointers...
	);
}

} // namespace cpu
} // namespace xmipp4
