// SPDX-License-Identifier: GPL-3.0-only

#include "reduction_element_adaptor.hpp"

#include "operand_pointers.hpp"

#include <xmipp4/core/platform/constexpr.hpp>

namespace xmipp4
{
namespace cpu
{

template <typename Kernel>
inline
reduction_element_adaptor<Kernel>::reduction_element_adaptor(
	Kernel kernel
)
	: m_kernel(std::move(kernel))
{
}

template <typename Kernel>
template <typename... Args>
inline
void reduction_element_adaptor<Kernel>::seed(Args &&...args) const
{
	m_kernel.seed(std::forward<Args>(args)...);
}

template <typename Kernel>
template <typename... Args>
inline
void reduction_element_adaptor<Kernel>::merge(Args &&...args) const
{
	m_kernel.merge(std::forward<Args>(args)...);
}

template <typename Kernel>
template <typename... Args>
inline
void reduction_element_adaptor<Kernel>::finalize(Args &&...args) const
{
	m_kernel.finalize(std::forward<Args>(args)...);
}

template <typename Kernel>
template <typename... Accumulators, typename K>
inline
auto reduction_element_adaptor<Kernel>::identity(
	Accumulators &...accumulators
) const
	-> decltype(std::declval<const K&>().identity(accumulators...))
{
	return m_kernel.identity(accumulators...);
}

template <typename Kernel>
template <typename... Accumulators, typename... Ins, typename... Strides>
inline
void reduction_element_adaptor<Kernel>::combine_run(
	const std::tuple<Accumulators*...> &accumulators,
	const std::tuple<const Ins*...> &inputs,
	const std::tuple<Strides...> &strides,
	std::size_t count,
	std::size_t position
) const
{
	combine_run(
		accumulators,
		inputs,
		strides,
		count,
		position,
		lane_folding(),
		std::index_sequence_for<Accumulators...>(),
		std::index_sequence_for<Ins...>()
	);
}

template <typename Kernel>
template <
	typename... Accumulators,
	typename... Ins,
	typename... Strides,
	std::size_t... As,
	std::size_t... Is
>
inline
void reduction_element_adaptor<Kernel>::combine_run(
	const std::tuple<Accumulators*...> &accumulators,
	const std::tuple<const Ins*...> &inputs,
	const std::tuple<Strides...> &strides,
	std::size_t count,
	std::size_t position,
	std::false_type,
	std::index_sequence<As...>,
	std::index_sequence<Is...> input_indices
) const
{
	for (std::size_t e = 0; e < count; ++e)
	{
		const auto element =
			detail::step_pointers(inputs, e, strides, input_indices);

		m_kernel.combine(
			*std::get<As>(accumulators)...,
			std::get<Is>(element)...,
			position + e
		);
	}
}

template <typename Kernel>
template <
	typename... Accumulators,
	typename... Ins,
	typename... Strides,
	std::size_t... As,
	std::size_t... Is
>
inline
void reduction_element_adaptor<Kernel>::combine_run(
	const std::tuple<Accumulators*...> &accumulators,
	const std::tuple<const Ins*...> &inputs,
	const std::tuple<Strides...> &strides,
	std::size_t count,
	std::size_t position,
	std::true_type,
	std::index_sequence<As...> accumulator_indices,
	std::index_sequence<Is...> input_indices
) const
{
	XMIPP4_CONST_CONSTEXPR std::size_t lanes =
		reduction_fold_lane_count<type_list<Accumulators...>>::value;

	// A run with less than one element per lane twice over has nothing to
	// deal: seeding the lanes and merging them back would cost more than the
	// independence is worth. See config.hpp for where that bound comes from.
	if (count < 2*lanes)
	{
		combine_run(
			accumulators,
			inputs,
			strides,
			count,
			position,
			std::false_type(),
			accumulator_indices,
			input_indices
		);
		return;
	}

	typename detail::accumulator_tiles<
		type_list<Accumulators...>,
		lanes
	>::type parts;

	// Every lane starts from an element of its own, for the reason the fold
	// as a whole is seeded from one rather than from an identity.
	for (std::size_t lane = 0; lane < lanes; ++lane)
	{
		const auto element =
			detail::step_pointers(inputs, lane, strides, input_indices);

		m_kernel.seed(
			std::get<As>(parts)[lane]...,
			std::get<Is>(element)...,
			position + lane
		);
	}

	// Consecutive elements to consecutive lanes, so that one deal reads one
	// contiguous stretch of each input.
	std::size_t e = lanes;
	for (; e + lanes <= count; e += lanes)
	{
		for (std::size_t lane = 0; lane < lanes; ++lane)
		{
			const auto element = detail::step_pointers(
				inputs,
				e + lane,
				strides,
				input_indices
			);

			m_kernel.combine(
				std::get<As>(parts)[lane]...,
				std::get<Is>(element)...,
				position + e + lane
			);
		}
	}

	// What is left is shorter than a deal, so it goes to the first lane
	// rather than to a partial one.
	for (; e < count; ++e)
	{
		const auto element =
			detail::step_pointers(inputs, e, strides, input_indices);

		m_kernel.combine(
			std::get<As>(parts)[0]...,
			std::get<Is>(element)...,
			position + e
		);
	}

	// Ascending in the lane index, which is the order the elements were
	// dealt in, so a merge that keeps the earlier of two equal answers keeps
	// the one it would have kept serially among the lanes it sees. Only a
	// kernel that also tolerates elements being dealt apart in the first
	// place reaches this at all.
	for (std::size_t lane = 0; lane < lanes; ++lane)
	{
		m_kernel.merge(
			*std::get<As>(accumulators)...,
			std::get<As>(parts)[lane]...
		);
	}
}

template <typename Kernel>
template <typename... Accumulators, typename... Ins, typename... Strides>
inline
void reduction_element_adaptor<Kernel>::combine_strip(
	const std::tuple<Accumulators*...> &accumulators,
	const std::tuple<const Ins*...> &inputs,
	const std::tuple<Strides...> &strides,
	std::size_t width,
	std::size_t position
) const
{
	combine_strip(
		accumulators,
		inputs,
		strides,
		width,
		position,
		std::index_sequence_for<Accumulators...>(),
		std::index_sequence_for<Ins...>()
	);
}

template <typename Kernel>
template <
	typename... Accumulators,
	typename... Ins,
	typename... Strides,
	std::size_t... As,
	std::size_t... Is
>
inline
void reduction_element_adaptor<Kernel>::combine_strip(
	const std::tuple<Accumulators*...> &accumulators,
	const std::tuple<const Ins*...> &inputs,
	const std::tuple<Strides...> &strides,
	std::size_t width,
	std::size_t position,
	std::index_sequence<As...>,
	std::index_sequence<Is...> input_indices
) const
{
	// No lanes here, and none needed: the strip is already as many
	// independent accumulators as it is wide, so the chain this breaks in
	// combine_run was never formed.
	for (std::size_t j = 0; j < width; ++j)
	{
		const auto element =
			detail::step_pointers(inputs, j, strides, input_indices);

		m_kernel.combine(
			std::get<As>(accumulators)[j]...,
			std::get<Is>(element)...,
			position
		);
	}
}

template <typename Kernel>
inline
reduction_element_adaptor<Kernel>
make_reduction_element_adaptor(Kernel kernel)
{
	return reduction_element_adaptor<Kernel>(std::move(kernel));
}

} // namespace cpu
} // namespace xmipp4
