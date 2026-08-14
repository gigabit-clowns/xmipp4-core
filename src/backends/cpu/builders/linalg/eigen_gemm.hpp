// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "linalg_core_layout_plan.hpp"

#include <xmipp4/core/numerical/fixed_width_float.hpp>

#include <Eigen/Dense>

#include <complex>
#include <cstddef>
#include <tuple>
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
	const linalg_operand_core &core
) noexcept
{
	if (core.get_rank() == 1)
	{
		return core.get_stride(0) == 1
			? core_layout_kind::vector_contiguous
			: core_layout_kind::generic;
	}

	if (core.get_stride(1) == 1 &&
	    core.get_stride(0) == static_cast<std::ptrdiff_t>(core.get_extent(1)))
	{
		return core_layout_kind::row_major_contiguous;
	}
	if (core.get_stride(0) == 1 &&
	    core.get_stride(1) == static_cast<std::ptrdiff_t>(core.get_extent(0)))
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
inline linalg_operand_core pad_as_column(const linalg_operand_core &vector_core) noexcept
{
	return linalg_operand_core(
		2,
		{ vector_core.get_extent(0), 1 },
		{ vector_core.get_stride(0), 1 }
	);
}

/**
 * @brief Pad a rank one core into a rank two row ({1, extent}).
 */
inline linalg_operand_core pad_as_row(const linalg_operand_core &vector_core) noexcept
{
	return linalg_operand_core(
		2,
		{ 1, vector_core.get_extent(0) },
		{ 1, vector_core.get_stride(0) }
	);
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
	const linalg_operand_core &out_core,
	const linalg_operand_core &left_core,
	const linalg_operand_core &right_core
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
 *
 * A 1x1 matrix is both at once, so neither constraint actually applies to
 * it; it is kept at the default (0) explicitly rather than falling through
 * to the general-matrix branch, since forcing a major order on a scalar
 * shape is what MSVC's Map rejects for a complex scalar type where GCC and
 * Clang happen to accept it.
 */
template <std::size_t Rows, std::size_t Cols, bool RowMajor>
struct eigen_matrix_options
	: std::integral_constant<int, RowMajor ? Eigen::RowMajor : Eigen::ColMajor>
{
};

template <bool RowMajor>
struct eigen_matrix_options<1, 1, RowMajor>
	: std::integral_constant<int, 0>
{
};

template <std::size_t Cols, bool RowMajor>
struct eigen_matrix_options<1, Cols, RowMajor>
	: std::integral_constant<int, static_cast<int>(Eigen::RowMajor)>
{
};

template <std::size_t Rows, bool RowMajor>
struct eigen_matrix_options<Rows, 1, RowMajor>
	: std::integral_constant<int, 0>
{
};

template <std::size_t M, std::size_t K, std::size_t N, bool RowMajor, typename T>
void gemm_call_fixed(
	T *out,
	const T *left,
	const T *right,
	const linalg_operand_core &/*out_core*/,
	const linalg_operand_core &/*left_core*/,
	const linalg_operand_core &/*right_core*/
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
	const linalg_operand_core &out_core,
	const linalg_operand_core &left_core,
	const linalg_operand_core &right_core
)
{
	using dynamic_matrix = Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic>;
	using stride_t = Eigen::Stride<Eigen::Dynamic, Eigen::Dynamic>;

	Eigen::Map<const dynamic_matrix, 0, stride_t> left_map(
		left,
		static_cast<Eigen::Index>(left_core.get_extent(0)),
		static_cast<Eigen::Index>(left_core.get_extent(1)),
		stride_t(left_core.get_stride(1), left_core.get_stride(0))
	);
	Eigen::Map<const dynamic_matrix, 0, stride_t> right_map(
		right,
		static_cast<Eigen::Index>(right_core.get_extent(0)),
		static_cast<Eigen::Index>(right_core.get_extent(1)),
		stride_t(right_core.get_stride(1), right_core.get_stride(0))
	);
	Eigen::Map<dynamic_matrix, 0, stride_t> out_map(
		out,
		static_cast<Eigen::Index>(out_core.get_extent(0)),
		static_cast<Eigen::Index>(out_core.get_extent(1)),
		stride_t(out_core.get_stride(1), out_core.get_stride(0))
	);
	out_map.noalias() = left_map * right_map;
}

static constexpr std::size_t max_fixed_extent = 4;

/**
 * @brief Smallest extent the fixed-size fast path is offered for.
 *
 * A dimension of exactly one means one of gemm_call_fixed's three
 * Eigen::Matrix instantiations is really a column vector ((extent, 1), as
 * matmul's own left operand becomes when K == 1); some MSVC toolsets fail
 * to even compile Eigen::Map over such a fixed-size matrix at all (a class
 * template instantiation failure, not a shape assertion, and reproducible
 * with a plain real scalar, so it is not a complex-specific issue and no
 * Options value works around it), where GCC and Clang have no trouble. The
 * fast path is skipped for that size everywhere rather than only where it
 * has been observed to fail, since the shapes it and the dynamic fallback
 * cover are otherwise identical.
 */
static constexpr std::size_t min_fixed_extent = 2;

/**
 * @brief Dispatch a runtime extent already known to be in [min_fixed_extent,
 * max_fixed_extent] to a compile time constant.
 *
 * @p f is invoked as `f(std::integral_constant<std::size_t, extent>{})`.
 * Instantiated for exactly the cases below, regardless of which one is
 * actually taken at runtime, which is what makes @ref min_fixed_extent an
 * actual bound on what gets compiled rather than merely on what a caller's
 * own range check happens to let through at runtime: a runtime guard cannot
 * stop this switch's own case labels from being instantiated, only from
 * being reached.
 */
template <typename F>
auto dispatch_fixed_extent(std::size_t n, F &&f)
	-> decltype(f(std::integral_constant<std::size_t, min_fixed_extent>{}))
{
	switch (n)
	{
		case 2: return f(std::integral_constant<std::size_t, 2>{});
		case 3: return f(std::integral_constant<std::size_t, 3>{});
		default: return f(std::integral_constant<std::size_t, 4>{});
	}
}

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
	if (m >= min_fixed_extent && m <= max_fixed_extent &&
	    k >= min_fixed_extent && k <= max_fixed_extent &&
	    n >= min_fixed_extent && n <= max_fixed_extent &&
	    all_row_major_compatible(out_kind, left_kind, right_kind))
	{
		return dispatch_fixed_extent(m, [&](auto m_c) {
			std::ignore = m_c; // Only its type, resolved via decltype, is used.
			return dispatch_fixed_extent(k, [&](auto k_c) {
				std::ignore = k_c;
				return dispatch_fixed_extent(n, [&](auto n_c) {
					std::ignore = n_c;
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
 * Always the dynamic path: the vector operand (right and output alike) is
 * always padded to an (extent, 1) shaped core (see pad_as_column), which is
 * exactly the fixed-size shape @ref min_fixed_extent exists to avoid, on
 * every call rather than only some, so there is no size worth fast-pathing
 * here at all.
 */
template <typename T>
gemm_fn<T> resolve_gemv(
	std::size_t /*m*/, std::size_t /*k*/,
	core_layout_kind /*out_kind*/,
	core_layout_kind /*left_kind*/,
	core_layout_kind /*right_kind*/
) noexcept
{
	return &gemm_call_dynamic<T>;
}

/**
 * @brief Resolve a K-vector times a (K, N) matrix.
 *
 * Always the dynamic path, for the same reason as @ref resolve_gemv: the
 * left and output operands are always padded to a (1, extent) shaped core
 * (see pad_as_row).
 */
template <typename T>
gemm_fn<T> resolve_vecgemm(
	std::size_t /*k*/, std::size_t /*n*/,
	core_layout_kind /*out_kind*/,
	core_layout_kind /*left_kind*/,
	core_layout_kind /*right_kind*/
) noexcept
{
	return &gemm_call_dynamic<T>;
}

} // namespace cpu
} // namespace xmipp4
