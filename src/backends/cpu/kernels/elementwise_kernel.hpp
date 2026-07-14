// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <backends/cpu/loops/inner_loop_stride_dispatch.hpp>

#include <cstddef>
#include <tuple>
#include <type_traits>
#include <utility>

namespace xmipp4
{
namespace cpu
{

/**
 * @brief Elementwise kernel applying an operation over a set of operands.
 *
 * Intended to be used as the kernel of @ref run_elementwise_loop, which
 * resolves each operand's inner stride and invokes the kernel once per 1D
 * vector as `kernel(pointers, strides, count)`. For each of the `count`
 * elements the operation is invoked with a pointer to the current element of
 * every operand:
 * @code
 * op(current0, current1, ...)
 * @endcode
 * where `currentK` points to the element of operand `K` for the current
 * iteration, i.e. its base pointer advanced by that operand's stride (for a
 * contiguous operand this is `&operandK[i]`, for a broadcasted operand it stays
 * at the first element, and for a runtime stride it is `operandK + i * stride`).
 * The pointers preserve each operand's cv-qualification (mutable for outputs,
 * `const` for inputs), so the operation itself decides which operands it writes
 * to; the kernel draws no distinction between inputs and outputs and imposes no
 * ordering between them.
 *
 * The loop is specialized on the resolved strides:
 * - an all-contiguous raw indexed loop for any operand count (the
 *   vectorization-friendly fast path and the natural place for a future SIMD
 *   specialization);
 * - a catch-all strided-iterator loop for any other stride combination. When
 *   the strides are statically known (contiguous or broadcasting tags) this
 *   loop vectorizes as well as the raw one, since the iterator carries the
 *   stride as a compile-time constant; only runtime strides fall back to
 *   scalar code.
 *
 * @tparam Op Operation invoked as `op(pointers...)` with one pointer per
 * operand, in the same order as the operands passed to
 * @ref run_elementwise_loop. Must be default-constructible or provided at
 * construction. May be stateful.
 */
template <typename Op>
class elementwise_kernel
{
public:
	/**
	 * @brief Construct a kernel with a default-constructed operation.
	 */
	elementwise_kernel() = default;

	/**
	 * @brief Construct a kernel holding the provided operation.
	 *
	 * @param op The operation to be applied.
	 */
	explicit elementwise_kernel(Op op);

	elementwise_kernel(const elementwise_kernel &other) = default;
	elementwise_kernel(elementwise_kernel &&other) = default;
	~elementwise_kernel() = default;

	elementwise_kernel& operator=(const elementwise_kernel &other) = default;
	elementwise_kernel& operator=(elementwise_kernel &&other) = default;

	/**
	 * @brief Apply the operation over the operands of a 1D vector.
	 *
	 * @tparam Pointers Pointee type of each operand pointer.
	 * @tparam Strides Stride type of each operand.
	 * @param pointers Tuple with one pointer per operand.
	 * @param strides Tuple with one stride per operand, in the same order.
	 * @param count Number of elements in the 1D vector.
	 */
	template <typename... Pointers, typename... Strides>
	void operator()(
		const std::tuple<Pointers*...> &pointers,
		const std::tuple<Strides...> &strides,
		std::size_t count
	) const;

private:
	Op m_op;

	// All operands contiguous: forward to the contiguous fast path.
	template <typename... Pointers, typename... Strides>
	void dispatch(
		const std::tuple<Pointers*...> &pointers,
		const std::tuple<Strides...> &strides,
		std::size_t count,
		std::true_type
	) const;

	// Otherwise: forward to a specialized or the generic loop.
	template <typename... Pointers, typename... Strides>
	void dispatch(
		const std::tuple<Pointers*...> &pointers,
		const std::tuple<Strides...> &strides,
		std::size_t count,
		std::false_type
	) const;

	// Index-sequence helper implementing the all-contiguous loop.
	template <typename... Pointers, std::size_t... Is>
	void contiguous_loop(
		const std::tuple<Pointers*...> &pointers,
		std::size_t count,
		std::index_sequence<Is...>
	) const;

	// Index-sequence helper implementing the generic strided loop.
	template <typename... Pointers, typename... Strides, std::size_t... Is>
	void generic_loop(
		const std::tuple<Pointers*...> &pointers,
		const std::tuple<Strides...> &strides,
		std::size_t count,
		std::index_sequence<Is...>
	) const;

};

/**
 * @brief Construct an @ref elementwise_kernel deducing its operation type.
 *
 * @tparam Op Operation type. Deduced from @p op.
 * @param op The operation to be applied.
 * @return elementwise_kernel<Op> The newly created kernel.
 */
template <typename Op>
elementwise_kernel<Op> make_elementwise_kernel(Op op);

} // namespace cpu
} // namespace xmipp4

#include "elementwise_kernel.inl"
