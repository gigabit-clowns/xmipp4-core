// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <array>

namespace xmipp4 
{
namespace multidimensional
{

template <std::size_t N>
struct cpu_loop
{
	using inner_loop = cpu_loop<N - 1>;

	template <typename Func, typename... Pointers>
	void operator()(
		const std::size_t *extents,
		const std::ptrdiff_t **strides,
		std::size_t level,
		const Func &func,
		Pointers... ptrs
	) const
	{
		loop_helper(
			extents,
			strides,
			level,
			func,
			ptrs...,
			std::make_index_sequence<sizeof...(Pointers)>()
		);
	}

private:
	template <typename Func, typename... Pointers, std::size_t... Is>
	void loop_helper(
		const std::size_t* extents,
		const std::ptrdiff_t** strides,
		std::size_t level,
		const Func &func,
		Pointers... ptrs,
		std::index_sequence<Is...>
	) const 
	{
		inner_loop inner;
		const auto next_level = level + 1;
		for (std::size_t i = 0; i < extents[level]; ++i)
		{
			inner(
				extents, 
				strides, 
				next_level,
				func,
				(ptrs + (i * extents[Is][level]))...
			);
		}
	}
};

template <>
struct cpu_loop<1>
{
	template <typename Func, typename... Pointers>
	void operator()(
		const std::size_t *extents,
		const std::ptrdiff_t **strides,
		std::size_t level,
		const Func &func,
		Pointers... ptrs
	) const
	{
		const auto last_extent = extents[level];
		const auto last_strides = get_strides(
			strides,
			level,
			std::make_index_sequence<sizeof...(Pointers)>()
		);

		func(last_extent, last_strides.data(), ptrs...);
	}

private:
	template <std::size_t... Is>
	std::array<std::ptrdiff_t, sizeof...(Is)> get_strides(
		const std::ptrdiff_t **strides,
		std::size_t level,
		std::index_sequence<Is...>
	) const
	{
		return { strides[Is][level]... };
	}

};

template <>
struct cpu_loop<0>
{
	template <typename Func, typename... Pointers>
	void operator()(
		const std::size_t*,
		const std::ptrdiff_t**,
		std::size_t,
		const Func &func,
		Pointers... ptrs
	) const
	{
		const std::size_t last_extent = 1UL;
		const std::array<std::ptrdiff_t, sizeof...(Pointers)> last_strides = {};
		func(last_extent, last_strides.data(), ptrs...);
	}
};


} // namespace multidimensional
} // namespace xmipp4
