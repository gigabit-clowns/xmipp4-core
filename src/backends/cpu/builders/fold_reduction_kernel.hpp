// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/meta/type_list.hpp>
#include <xmipp4/core/platform/cpp_attributes.hpp>

#include <backends/cpu/load_store.hpp>

#include <cstddef>
#include <type_traits>
#include <utility>

namespace xmipp4
{
namespace cpu
{

// This header holds scaffolding rather than arithmetic: it adapts a plain
// binary fold to the reduction kernel concept, the way default_kernel_factory
// adapts a stateless kernel to the factory one. That is why it sits beside
// the builders and not in kernels/, which is for the arithmetic itself: a
// fold's own body belongs in its operation's translation unit, or in kernels/
// when several operations share it.

/**
 * @brief The type an element of storage type T is computed in.
 *
 * Half precision cannot be operated on directly, so it is widened on load and
 * rounded back on store. An accumulator is typed on the computation type
 * rather than on the storage type, which is what keeps a reduction of half
 * precision data from accumulating rounding error at every step.
 *
 * @tparam T The storage element type.
 */
template <typename T>
struct reduction_compute_type
{
	using type = decltype(load(std::declval<const T*>()));
};

/**
 * @brief Reduction kernel built from a binary fold.
 *
 * Covers the operations whose whole definition is one associative binary
 * operation with a neutral element: a fold supplies the combination and the
 * identity, and everything else follows. The accumulator is the computation
 * type of the output, so the declared typing rule remains the only thing
 * deciding what a reduction accumulates in.
 *
 * A fold is invoked as `fold(accumulator, value)` and must additionally
 * provide `Fold::identity<T>()`. Operations needing a transformation on the
 * way in, a post processing on the way out or more than one accumulator
 * spell those members out themselves rather than using this adaptor.
 *
 * @tparam Fold The binary fold. Must be default constructible.
 */
template <typename Fold>
class fold_reduction_kernel
{
public:
	/**
	 * @brief A single accumulator, of the output's computation type.
	 *
	 * @tparam Outputs type_list of the output element types.
	 * @tparam Inputs type_list of the input element types.
	 */
	template <typename Outputs, typename Inputs>
	struct accumulators
	{
		using type = type_list<
			typename reduction_compute_type<
				typename type_list_element<0, Outputs>::type
			>::type
		>;
	};

	template <typename Accumulator, typename T>
	void seed(Accumulator &accumulator, const T *value) const noexcept
	{
		accumulator = static_cast<Accumulator>(load(value));
	}

	template <typename Accumulator, typename T>
	void combine(Accumulator &accumulator, const T *value) const noexcept
	{
		accumulator = m_fold(
			accumulator,
			static_cast<Accumulator>(load(value))
		);
	}

	template <typename Accumulator>
	void merge(
		Accumulator &accumulator,
		const Accumulator &other
	) const noexcept
	{
		accumulator = m_fold(accumulator, other);
	}

	template <typename U, typename Accumulator>
	void finalize(
		U *result,
		const Accumulator &accumulator,
		std::size_t /*count*/
	) const noexcept
	{
		store(result, accumulator);
	}

	/**
	 * @brief The neutral element of the fold, when it has one.
	 *
	 * The return type is spelled in terms of the fold's own identity so that
	 * this member disappears for a fold without one, rather than failing to
	 * compile when the loop asks for it. A reduction over no elements is
	 * then rejected at the one place that can tell the difference.
	 *
	 * The fold is bound to a parameter of this member rather than being read
	 * from the enclosing class, so that the lookup happens while the member
	 * is being substituted and a missing identity is a deduction failure
	 * instead of an error.
	 */
	template <typename Accumulator, typename F = Fold>
	auto identity(Accumulator &accumulator) const noexcept
		-> decltype(static_cast<void>(F::template identity<Accumulator>()))
	{
		accumulator = F::template identity<Accumulator>();
	}

private:
	XMIPP4_NO_UNIQUE_ADDRESS Fold m_fold;
};

} // namespace cpu
} // namespace xmipp4
