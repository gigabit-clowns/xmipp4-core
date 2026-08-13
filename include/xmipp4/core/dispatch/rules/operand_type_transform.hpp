// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "../../numerical/numerical_type.hpp"
#include "../../numerical/numerical_type_domain.hpp"
#include "../../numerical/numerical_type_traits.hpp"

#include <complex>

namespace xmipp4
{

/**
 * @defgroup operand_type_transform Operand type transforms
 *
 * A transform maps the type of a rule's pivot onto the type of one operand.
 *
 * Every transform states the same mapping twice:
 *
 *   * `apply` evaluates it on runtime numerical_type values. The core data
 *     type policy reads this half to deduce and check operand types.
 *   * `apply_type` evaluates it on native C++ types. Backend type
 *     dispatchers read this half to reify the operand element types once
 *     the pivots have been resolved.
 *
 * The two halves must agree: for every native type T admitted by
 * `domain_type`,
 *
 *     numerical_type_of<typename apply_type<T>::type>::value ==
 *         apply(numerical_type_of<T>::value)
 *
 * This is machine checked for every transform by the transform consistency
 * test, which is what makes it safe to write a typing rule once and have
 * both sides interpret it.
 *
 * A transform also declares `domain_type`, the set of pivot types it is
 * defined on. A rule intersects it with the domain the operation declares,
 * so a rule can never resolve to a type its own transforms cannot map.
 *
 * `get_relation` returns a fragment naming the mapping, used to build
 * diagnostics such as "output operand 'result' must be the real
 * counterpart of input operand 'value'".
 *
 * @{
 */

/**
 * @brief Transform mapping a pivot type onto itself.
 *
 * Models operations whose operands all share one type, such as `add`.
 */
struct same_type_transform
{
	using domain_type = any_type_domain;

	static const char* get_relation() noexcept
	{
		return "the same type as";
	}

	static numerical_type apply(numerical_type type) noexcept
	{
		return type;
	}

	template <typename T>
	struct apply_type
	{
		using type = T;
	};
};

/**
 * @brief Transform mapping a pivot type onto its real counterpart.
 *
 * Non complex types are left untouched, so this is defined on every type.
 * Models operations that drop the imaginary part, such as `abs` or `real`.
 */
struct real_type_transform
{
	using domain_type = any_type_domain;

	static const char* get_relation() noexcept
	{
		return "the real counterpart of";
	}

	static numerical_type apply(numerical_type type) noexcept
	{
		return make_real(type);
	}

	template <typename T>
	struct apply_type
	{
		using type = T;
	};

	template <typename T>
	struct apply_type<std::complex<T>>
	{
		using type = T;
	};
};

/**
 * @brief Transform mapping a pivot type onto its complex counterpart.
 *
 * Only floating point and complex types have one, hence the restricted
 * domain. Models operations that introduce an imaginary part.
 */
struct complex_type_transform
{
	using domain_type = inexact_type_domain;

	static const char* get_relation() noexcept
	{
		return "the complex counterpart of";
	}

	static numerical_type apply(numerical_type type) noexcept
	{
		return make_complex(type);
	}

	template <typename T>
	struct apply_type
	{
		using type = std::complex<T>;
	};

	template <typename T>
	struct apply_type<std::complex<T>>
	{
		using type = std::complex<T>;
	};
};

/**
 * @brief Transform ignoring the pivot and yielding a fixed type.
 *
 * Models operations whose operand type does not depend on the others, such
 * as the boolean output of a comparison.
 *
 * @tparam Type The produced numerical type.
 */
template <numerical_type Type>
struct fixed_type_transform
{
	using domain_type = any_type_domain;

	static const char* get_relation() noexcept
	{
		return "independent of";
	}

	static numerical_type apply(numerical_type) noexcept
	{
		return Type;
	}

	template <typename T>
	struct apply_type
	{
		using type = typename native_type_of<Type>::type;
	};
};

/** @} */

} // namespace xmipp4
