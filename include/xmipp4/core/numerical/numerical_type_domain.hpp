// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "numerical_type.hpp"

#include "../platform/constexpr.hpp"
#include "../platform/dynamic_shared_object.h"

#include <cstdint>
#include <initializer_list>
#include <ostream>

namespace xmipp4
{

/**
 * @brief Set of numerical_type-s.
 *
 * Represents which numerical types are admissible in a given context, such
 * as the operand types an operation accepts or the subset of those a
 * backend is able to execute. Domains compose with the usual set
 * operations, which lets a backend narrow an operation's domain without
 * restating it.
 *
 * The set is stored as a bit mask indexed by the numerical_type
 * enumerator, so it is a trivially copyable value type.
 *
 * @see numerical_type
 */
class numerical_type_domain
{
public:
	/**
	 * @brief Underlying bit mask representation.
	 */
	using mask_type = std::uint32_t;

	/**
	 * @brief Construct an empty domain.
	 */
	XMIPP4_CONSTEXPR
	numerical_type_domain() noexcept;

	/**
	 * @brief Construct a domain from its bit mask representation.
	 *
	 * @param mask The bit mask, indexed by numerical_type enumerator.
	 */
	XMIPP4_CONSTEXPR
	explicit numerical_type_domain(mask_type mask) noexcept;

	/**
	 * @brief Construct a domain from an explicit list of types.
	 *
	 * Types with no bit representation, such as numerical_type::unknown,
	 * are ignored.
	 *
	 * @param types The types belonging to the domain.
	 */
	XMIPP4_CONSTEXPR_CPP14
	numerical_type_domain(
		std::initializer_list<numerical_type> types
	) noexcept;

	numerical_type_domain(const numerical_type_domain &other) = default;
	numerical_type_domain(numerical_type_domain &&other) noexcept = default;
	~numerical_type_domain() = default;

	numerical_type_domain&
	operator=(const numerical_type_domain &other) = default;
	numerical_type_domain&
	operator=(numerical_type_domain &&other) noexcept = default;

	/**
	 * @brief Check whether a type belongs to this domain.
	 *
	 * @param type The type to be tested.
	 * @return true The type belongs to the domain.
	 * @return false The type does not belong to the domain, or has no bit
	 * representation.
	 */
	XMIPP4_CONSTEXPR
	bool contains(numerical_type type) const noexcept;

	/**
	 * @brief Check whether the domain admits no type at all.
	 *
	 * @return true The domain is empty.
	 * @return false The domain admits at least one type.
	 */
	XMIPP4_CONSTEXPR
	bool empty() const noexcept;

	/**
	 * @brief Get the bit mask representation of the domain.
	 *
	 * @return mask_type The bit mask.
	 */
	XMIPP4_CONSTEXPR
	mask_type get_mask() const noexcept;

	/**
	 * @brief Create the domain containing every representable type.
	 *
	 * @return numerical_type_domain The full domain.
	 */
	static XMIPP4_CONSTEXPR
	numerical_type_domain all() noexcept;

	/**
	 * @brief Create the domain containing no type.
	 *
	 * @return numerical_type_domain The empty domain.
	 */
	static XMIPP4_CONSTEXPR
	numerical_type_domain none() noexcept;

private:
	mask_type m_mask;
};

/**
 * @brief Check whether a type has a bit representation within a domain.
 *
 * Only the enumerators in [numerical_type::boolean, numerical_type::count)
 * are representable. In particular numerical_type::unknown is not.
 *
 * @param type The type to be tested.
 * @return true The type is representable.
 * @return false The type is not representable.
 */
XMIPP4_CONSTEXPR
bool is_domain_representable(numerical_type type) noexcept;

XMIPP4_CONSTEXPR
bool operator==(
	const numerical_type_domain &lhs,
	const numerical_type_domain &rhs
) noexcept;

XMIPP4_CONSTEXPR
bool operator!=(
	const numerical_type_domain &lhs,
	const numerical_type_domain &rhs
) noexcept;

/**
 * @brief Compute the intersection of two domains.
 */
XMIPP4_CONSTEXPR
numerical_type_domain operator&(
	const numerical_type_domain &lhs,
	const numerical_type_domain &rhs
) noexcept;

/**
 * @brief Compute the union of two domains.
 */
XMIPP4_CONSTEXPR
numerical_type_domain operator|(
	const numerical_type_domain &lhs,
	const numerical_type_domain &rhs
) noexcept;

/**
 * @brief Compute the complement of a domain.
 *
 * The complement is taken with respect to the set of all representable
 * types, so no unrepresentable bit is ever set.
 */
XMIPP4_CONSTEXPR
numerical_type_domain operator~(const numerical_type_domain &domain) noexcept;

/**
 * @brief Create the domain holding every type of a given category.
 *
 * @param category The category of the types belonging to the domain.
 * @return numerical_type_domain The domain.
 *
 * @see numerical_type_category
 */
XMIPP4_CORE_API
numerical_type_domain make_numerical_type_domain(
	numerical_type_category category
) noexcept;

/**
 * @brief Write a human readable representation of a domain.
 *
 * The types are listed in enumerator order, separated by commas.
 *
 * @param os The stream where the domain is written.
 * @param domain The domain to be written.
 * @return std::ostream& The stream.
 */
XMIPP4_CORE_API
std::ostream& operator<<(
	std::ostream &os,
	const numerical_type_domain &domain
);



/**
 * @brief Named domain holding every type of a given category.
 *
 * Domains are named by tag types rather than by values, so that they can be
 * used as template arguments. Every tag exposes a static get() returning a
 * reference to the domain it names.
 *
 * @tparam Category The category of the types belonging to the domain.
 */
template <numerical_type_category Category>
struct category_type_domain
{
	static const numerical_type_domain& get() noexcept;
};

using boolean_type_domain =
	category_type_domain<numerical_type_category::boolean>;
using character_type_domain =
	category_type_domain<numerical_type_category::character>;
using signed_integer_type_domain =
	category_type_domain<numerical_type_category::signed_integer>;
using unsigned_integer_type_domain =
	category_type_domain<numerical_type_category::unsigned_integer>;
using floating_point_type_domain =
	category_type_domain<numerical_type_category::floating_point>;
using complex_type_domain =
	category_type_domain<numerical_type_category::complex>;

/**
 * @brief Named domain holding the union of other named domains.
 *
 * @tparam Domains The domain tags to be united. At least one is required.
 */
template <typename... Domains>
struct domain_union
{
	static const numerical_type_domain& get() noexcept;
};

/**
 * @brief Named domain holding the intersection of other named domains.
 *
 * This is how a backend narrows the domain of an operation without
 * restating it.
 *
 * @tparam Domains The domain tags to be intersected. At least one is
 * required.
 */
template <typename... Domains>
struct domain_intersection
{
	static const numerical_type_domain& get() noexcept;
};

/**
 * @brief Named domain holding every representable type.
 */
struct any_type_domain
{
	static const numerical_type_domain& get() noexcept;
};

/// Signed and unsigned integers.
using integral_type_domain = domain_union<
	signed_integer_type_domain,
	unsigned_integer_type_domain
>;

/// Floating point and complex types, i.e. those with a fractional part.
using inexact_type_domain = domain_union<
	floating_point_type_domain,
	complex_type_domain
>;

/// Integers and floating point types, i.e. arithmetic types on the real line.
using real_arithmetic_type_domain = domain_union<
	integral_type_domain,
	floating_point_type_domain
>;

/// Every type arithmetic operations are defined on.
using arithmetic_type_domain = domain_union<
	integral_type_domain,
	inexact_type_domain
>;

/// Arithmetic types that can represent a negative value.
using signed_arithmetic_type_domain = domain_union<
	signed_integer_type_domain,
	inexact_type_domain
>;

} // namespace xmipp4

#include "numerical_type_domain.inl"
