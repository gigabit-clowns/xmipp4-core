// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "linalg_core_layout_plan.hpp"

#include <xmipp4/core/numerical/fixed_width_float.hpp>

#include <Eigen/Dense>

#include <complex>
#include <cstddef>
#include <type_traits>

namespace xmipp4
{
namespace cpu
{

/**
 * @brief Predicate excluding the types Eigen cannot be handed as a scalar.
 *
 * xmipp4's own float16_t (and its complex counterpart) is not a type Eigen
 * knows how to specialize its numeric traits for; every other type
 * arithmetic_type_domain admits (the fixed-width integers, float, double,
 * and their complex counterparts) is a native Eigen scalar type.
 */
template <typename T>
struct eigen_scalar_support
	: std::integral_constant<
		bool,
		!std::is_same<T, float16_t>::value &&
		!std::is_same<T, std::complex<float16_t>>::value
	>
{
};

/**
 * @brief How an operand's core axes sit in memory.
 *
 * A vector core (rank one) has no major order of its own: a contiguous
 * vector is equally a contiguous row or a contiguous column, so it is
 * tagged separately and treated as compatible with either.
 */
enum class core_layout_kind
{
	row_major_contiguous,
	column_major_contiguous,
	vector_contiguous,
	generic
};

inline core_layout_kind classify_core_layout(
	const linalg_core_layout_plan::operand_core &core
) noexcept
{
	if (core.rank == 1)
	{
		return core.strides[0] == 1
			? core_layout_kind::vector_contiguous
			: core_layout_kind::generic;
	}

	if (core.strides[1] == 1 &&
	    core.strides[0] == static_cast<std::ptrdiff_t>(core.extents[1]))
	{
		return core_layout_kind::row_major_contiguous;
	}
	if (core.strides[0] == 1 &&
	    core.strides[1] == static_cast<std::ptrdiff_t>(core.extents[0]))
	{
		return core_layout_kind::column_major_contiguous;
	}
	return core_layout_kind::generic;
}

inline bool is_row_major_compatible(core_layout_kind kind) noexcept
{
	return kind == core_layout_kind::row_major_contiguous ||
	       kind == core_layout_kind::vector_contiguous;
}

inline bool is_column_major_compatible(core_layout_kind kind) noexcept
{
	return kind == core_layout_kind::column_major_contiguous ||
	       kind == core_layout_kind::vector_contiguous;
}

/**
 * @brief Pad a rank one core into a rank two column ({extent, 1}).
 *
 * The padding axis has extent one, so its stride is never dereferenced;
 * any value is safe.
 */
inline linalg_core_layout_plan::operand_core pad_as_column(
	const linalg_core_layout_plan::operand_core &vector_core
) noexcept
{
	linalg_core_layout_plan::operand_core core;
	core.rank = 2;
	core.extents = { vector_core.extents[0], 1 };
	core.strides = { vector_core.strides[0], 1 };
	return core;
}

/**
 * @brief Pad a rank one core into a rank two row ({1, extent}).
 */
inline linalg_core_layout_plan::operand_core pad_as_row(
	const linalg_core_layout_plan::operand_core &vector_core
) noexcept
{
	linalg_core_layout_plan::operand_core core;
	core.rank = 2;
	core.extents = { 1, vector_core.extents[0] };
	core.strides = { 1, vector_core.strides[0] };
	return core;
}

/**
 * @brief Signature every gemm implementation shares, fixed size or not.
 *
 * Resolved once per program (in a kernel's constructor) into a stored
 * function pointer, so a batch loop invoking the kernel many times pays no
 * further dispatch cost.
 */
template <typename T>
using gemm_fn = void (*)(
	T *out,
	const T *left,
	const T *right,
	const linalg_core_layout_plan::operand_core &out_core,
	const linalg_core_layout_plan::operand_core &left_core,
	const linalg_core_layout_plan::operand_core &right_core
);

/**
 * @brief Fully unrolled small GEMM over contiguous, known-major operands.
 *
 * The core extents/strides are not consulted: the caller has already
 * verified the operands are contiguous of the stated major order and sized
 * exactly (M, K, N).
 */
/**
 * @brief The storage option a fixed (Rows, Cols) Eigen::Matrix may take.
 *
 * Eigen requires, rather than merely allows, a specific option on a matrix
 * that is really a vector: a row vector (Rows == 1, Cols > 1) must have the
 * RowMajor bit set, and a column vector (Cols == 1, Rows > 1) must not.
 * Getting this backwards, or leaving it at the caller's general-matrix
 * choice, fails INVALID_MATRIX_TEMPLATE_PARAMETERS.
 */
template <std::size_t Rows, std::size_t Cols, bool RowMajor>
struct eigen_matrix_options
	: std::integral_constant<
		int,
		(Cols == 1 && Rows != 1)
			? 0
			: (Rows == 1 && Cols != 1)
				? static_cast<int>(Eigen::RowMajor)
				: (RowMajor
					? static_cast<int>(Eigen::RowMajor)
					: static_cast<int>(Eigen::ColMajor))
	>
{
};

template <std::size_t M, std::size_t K, std::size_t N, bool RowMajor, typename T>
void gemm_call_fixed(
	T *out,
	const T *left,
	const T *right,
	const linalg_core_layout_plan::operand_core &/*out_core*/,
	const linalg_core_layout_plan::operand_core &/*left_core*/,
	const linalg_core_layout_plan::operand_core &/*right_core*/
)
{
	using left_matrix =
		Eigen::Matrix<T, M, K, eigen_matrix_options<M, K, RowMajor>::value>;
	using right_matrix =
		Eigen::Matrix<T, K, N, eigen_matrix_options<K, N, RowMajor>::value>;
	using out_matrix =
		Eigen::Matrix<T, M, N, eigen_matrix_options<M, N, RowMajor>::value>;

	Eigen::Map<const left_matrix> left_map(left);
	Eigen::Map<const right_matrix> right_map(right);
	Eigen::Map<out_matrix> out_map(out);
	out_map.noalias() = left_map * right_map;
}

/**
 * @brief GEMM over operands of arbitrary, runtime-known strides and sizes.
 *
 * The one fallback every shape and layout combination outside the fixed
 * fast path uses.
 */
template <typename T>
void gemm_call_dynamic(
	T *out,
	const T *left,
	const T *right,
	const linalg_core_layout_plan::operand_core &out_core,
	const linalg_core_layout_plan::operand_core &left_core,
	const linalg_core_layout_plan::operand_core &right_core
)
{
	using dynamic_matrix = Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic>;
	using stride_t = Eigen::Stride<Eigen::Dynamic, Eigen::Dynamic>;

	Eigen::Map<const dynamic_matrix, 0, stride_t> left_map(
		left,
		static_cast<Eigen::Index>(left_core.extents[0]),
		static_cast<Eigen::Index>(left_core.extents[1]),
		stride_t(left_core.strides[1], left_core.strides[0])
	);
	Eigen::Map<const dynamic_matrix, 0, stride_t> right_map(
		right,
		static_cast<Eigen::Index>(right_core.extents[0]),
		static_cast<Eigen::Index>(right_core.extents[1]),
		stride_t(right_core.strides[1], right_core.strides[0])
	);
	Eigen::Map<dynamic_matrix, 0, stride_t> out_map(
		out,
		static_cast<Eigen::Index>(out_core.extents[0]),
		static_cast<Eigen::Index>(out_core.extents[1]),
		stride_t(out_core.strides[1], out_core.strides[0])
	);
	out_map.noalias() = left_map * right_map;
}

/**
 * @brief Dispatch a runtime extent already known to be in [1, 4] to a
 * compile time constant.
 *
 * @p f is invoked as `f(std::integral_constant<std::size_t, extent>{})`.
 * Instantiated for exactly the four cases below, regardless of which one is
 * actually taken at runtime; the caller must ensure @p n is one of them.
 */
template <typename F>
auto dispatch_fixed_extent(std::size_t n, F &&f)
	-> decltype(f(std::integral_constant<std::size_t, 1>{}))
{
	switch (n)
	{
		case 1: return f(std::integral_constant<std::size_t, 1>{});
		case 2: return f(std::integral_constant<std::size_t, 2>{});
		case 3: return f(std::integral_constant<std::size_t, 3>{});
		default: return f(std::integral_constant<std::size_t, 4>{});
	}
}

static constexpr std::size_t max_fixed_extent = 4;

/**
 * @brief Types worth instantiating the fixed-size fast path for.
 *
 * eigen_scalar_support decides what this backend executes at all; this is
 * the separate, narrower question of what is worth the compile time cost of
 * the fixed-size dispatch grid below. Every other supported type (the
 * fixed-width integers) still executes correctly, through gemm_call_dynamic
 * alone, just without the unrolled fast path.
 */
template <typename T>
struct eigen_fixed_path_support
	: std::integral_constant<
		bool,
		std::is_same<T, float>::value ||
		std::is_same<T, double>::value ||
		std::is_same<T, std::complex<float>>::value ||
		std::is_same<T, std::complex<double>>::value
	>
{
};

/**
 * @brief Whether every one of a batch of operand kinds is row-major
 * contiguous (a vector core is compatible with either major order).
 *
 * Only row-major is fast-pathed: this codebase's layouts default to it, and
 * a column-major operand is still correct, just through the dynamic path,
 * which halves the size of the dispatch grid below for a case this backend
 * does not expect to be common.
 */
inline bool all_row_major_compatible(
	core_layout_kind a, core_layout_kind b, core_layout_kind c
) noexcept
{
	return is_row_major_compatible(a) &&
	       is_row_major_compatible(b) &&
	       is_row_major_compatible(c);
}

/**
 * @brief Resolve a (M, K, N) shaped product, for a type worth fast-pathing.
 */
template <typename T>
gemm_fn<T> resolve_gemm_fixed_path(
	std::size_t m, std::size_t k, std::size_t n,
	core_layout_kind out_kind,
	core_layout_kind left_kind,
	core_layout_kind right_kind
)
{
	if (m >= 1 && m <= max_fixed_extent &&
	    k >= 1 && k <= max_fixed_extent &&
	    n >= 1 && n <= max_fixed_extent &&
	    all_row_major_compatible(out_kind, left_kind, right_kind))
	{
		return dispatch_fixed_extent(m, [&](auto m_c) {
			return dispatch_fixed_extent(k, [&](auto k_c) {
				return dispatch_fixed_extent(n, [&](auto n_c) {
					return &gemm_call_fixed<
						decltype(m_c)::value,
						decltype(k_c)::value,
						decltype(n_c)::value,
						true,
						T
					>;
				});
			});
		});
	}

	return &gemm_call_dynamic<T>;
}

/**
 * @brief Resolve a (M, K) matrix times a K-vector, for a type worth
 * fast-pathing. N is always one: dispatching it as if it varied would
 * needlessly triple the size of the grid below.
 */
template <typename T>
gemm_fn<T> resolve_gemv_fixed_path(
	std::size_t m, std::size_t k,
	core_layout_kind out_kind,
	core_layout_kind left_kind,
	core_layout_kind right_kind
)
{
	if (m >= 1 && m <= max_fixed_extent &&
	    k >= 1 && k <= max_fixed_extent &&
	    all_row_major_compatible(out_kind, left_kind, right_kind))
	{
		return dispatch_fixed_extent(m, [&](auto m_c) {
			return dispatch_fixed_extent(k, [&](auto k_c) {
				return &gemm_call_fixed<
					decltype(m_c)::value, decltype(k_c)::value, 1, true, T
				>;
			});
		});
	}

	return &gemm_call_dynamic<T>;
}

/**
 * @brief Resolve a K-vector times a (K, N) matrix, for a type worth
 * fast-pathing. M is always one, for the same reason as resolve_gemv_fixed_path.
 */
template <typename T>
gemm_fn<T> resolve_vecgemm_fixed_path(
	std::size_t k, std::size_t n,
	core_layout_kind out_kind,
	core_layout_kind left_kind,
	core_layout_kind right_kind
)
{
	if (k >= 1 && k <= max_fixed_extent &&
	    n >= 1 && n <= max_fixed_extent &&
	    all_row_major_compatible(out_kind, left_kind, right_kind))
	{
		return dispatch_fixed_extent(k, [&](auto k_c) {
			return dispatch_fixed_extent(n, [&](auto n_c) {
				return &gemm_call_fixed<
					1, decltype(k_c)::value, decltype(n_c)::value, true, T
				>;
			});
		});
	}

	return &gemm_call_dynamic<T>;
}

/**
 * @brief Resolve which gemm implementation a (M, K, N) shaped product with
 * the given operand layouts should use, once, up front.
 *
 * For a type not worth the fixed-size dispatch grid (see
 * eigen_fixed_path_support), that whole grid is skipped at compile time:
 * resolve_gemm_fixed_path is never instantiated for it, so none of its
 * Eigen::Matrix specializations are either.
 */
template <typename T>
gemm_fn<T> resolve_gemm(
	std::size_t /*m*/, std::size_t /*k*/, std::size_t /*n*/,
	core_layout_kind /*out_kind*/,
	core_layout_kind /*left_kind*/,
	core_layout_kind /*right_kind*/,
	std::false_type /* eigen_fixed_path_support<T> */
)
{
	return &gemm_call_dynamic<T>;
}

template <typename T>
gemm_fn<T> resolve_gemm(
	std::size_t m, std::size_t k, std::size_t n,
	core_layout_kind out_kind,
	core_layout_kind left_kind,
	core_layout_kind right_kind,
	std::true_type /* eigen_fixed_path_support<T> */
)
{
	return resolve_gemm_fixed_path<T>(m, k, n, out_kind, left_kind, right_kind);
}

template <typename T>
gemm_fn<T> resolve_gemm(
	std::size_t m, std::size_t k, std::size_t n,
	core_layout_kind out_kind,
	core_layout_kind left_kind,
	core_layout_kind right_kind
)
{
	return resolve_gemm<T>(
		m, k, n, out_kind, left_kind, right_kind,
		eigen_fixed_path_support<T>{}
	);
}

/**
 * @brief Resolve a (M, K) matrix times a K-vector.
 *
 * @see resolve_gemm
 */
template <typename T>
gemm_fn<T> resolve_gemv(
	std::size_t /*m*/, std::size_t /*k*/,
	core_layout_kind /*out_kind*/,
	core_layout_kind /*left_kind*/,
	core_layout_kind /*right_kind*/,
	std::false_type /* eigen_fixed_path_support<T> */
)
{
	return &gemm_call_dynamic<T>;
}

template <typename T>
gemm_fn<T> resolve_gemv(
	std::size_t m, std::size_t k,
	core_layout_kind out_kind,
	core_layout_kind left_kind,
	core_layout_kind right_kind,
	std::true_type /* eigen_fixed_path_support<T> */
)
{
	return resolve_gemv_fixed_path<T>(m, k, out_kind, left_kind, right_kind);
}

template <typename T>
gemm_fn<T> resolve_gemv(
	std::size_t m, std::size_t k,
	core_layout_kind out_kind,
	core_layout_kind left_kind,
	core_layout_kind right_kind
)
{
	return resolve_gemv<T>(
		m, k, out_kind, left_kind, right_kind, eigen_fixed_path_support<T>{}
	);
}

/**
 * @brief Resolve a K-vector times a (K, N) matrix.
 *
 * @see resolve_gemm
 */
template <typename T>
gemm_fn<T> resolve_vecgemm(
	std::size_t /*k*/, std::size_t /*n*/,
	core_layout_kind /*out_kind*/,
	core_layout_kind /*left_kind*/,
	core_layout_kind /*right_kind*/,
	std::false_type /* eigen_fixed_path_support<T> */
)
{
	return &gemm_call_dynamic<T>;
}

template <typename T>
gemm_fn<T> resolve_vecgemm(
	std::size_t k, std::size_t n,
	core_layout_kind out_kind,
	core_layout_kind left_kind,
	core_layout_kind right_kind,
	std::true_type /* eigen_fixed_path_support<T> */
)
{
	return resolve_vecgemm_fixed_path<T>(k, n, out_kind, left_kind, right_kind);
}

template <typename T>
gemm_fn<T> resolve_vecgemm(
	std::size_t k, std::size_t n,
	core_layout_kind out_kind,
	core_layout_kind left_kind,
	core_layout_kind right_kind
)
{
	return resolve_vecgemm<T>(
		k, n, out_kind, left_kind, right_kind, eigen_fixed_path_support<T>{}
	);
}

} // namespace cpu
} // namespace xmipp4
