// SPDX-License-Identifier: GPL-3.0-only

#include "numerical_type_domain.hpp"

namespace xmipp4
{

namespace detail
{

static_assert(
	static_cast<int>(numerical_type::count) <=
	static_cast<int>(sizeof(numerical_type_domain::mask_type) * 8),
	"numerical_type no longer fits in a numerical_type_domain mask. Widen "
	"numerical_type_domain::mask_type."
);

/**
 * @brief Get the bit representing a type within a domain mask.
 *
 * @param type The type. Must be representable.
 * @return numerical_type_domain::mask_type The bit.
 */
XMIPP4_INLINE_CONSTEXPR
numerical_type_domain::mask_type
numerical_type_domain_bit(numerical_type type) noexcept
{
	return numerical_type_domain::mask_type(1) << static_cast<int>(type);
}

/**
 * @brief Get the mask holding every representable type.
 *
 * @return numerical_type_domain::mask_type The mask.
 */
XMIPP4_INLINE_CONSTEXPR
numerical_type_domain::mask_type
numerical_type_domain_all_bits() noexcept
{
	return (
		numerical_type_domain::mask_type(1) <<
		static_cast<int>(numerical_type::count)
	) - numerical_type_domain::mask_type(1);
}

} // namespace detail

XMIPP4_INLINE_CONSTEXPR
bool is_domain_representable(numerical_type type) noexcept
{
	return static_cast<int>(type) >= 0 &&
	       static_cast<int>(type) < static_cast<int>(numerical_type::count);
}

XMIPP4_INLINE_CONSTEXPR
numerical_type_domain::numerical_type_domain() noexcept
	: m_mask(0)
{
}

XMIPP4_INLINE_CONSTEXPR
numerical_type_domain::numerical_type_domain(mask_type mask) noexcept
	: m_mask(mask)
{
}

XMIPP4_INLINE_CONSTEXPR_CPP14
numerical_type_domain::numerical_type_domain(
	std::initializer_list<numerical_type> types
) noexcept
	: m_mask(0)
{
	for (const auto type : types)
	{
		if (is_domain_representable(type))
		{
			m_mask |= detail::numerical_type_domain_bit(type);
		}
	}
}

XMIPP4_INLINE_CONSTEXPR
bool numerical_type_domain::contains(numerical_type type) const noexcept
{
	return is_domain_representable(type) &&
	       (m_mask & detail::numerical_type_domain_bit(type)) != 0;
}

XMIPP4_INLINE_CONSTEXPR
bool numerical_type_domain::empty() const noexcept
{
	return m_mask == 0;
}

XMIPP4_INLINE_CONSTEXPR
numerical_type_domain::mask_type
numerical_type_domain::get_mask() const noexcept
{
	return m_mask;
}

XMIPP4_INLINE_CONSTEXPR
numerical_type_domain numerical_type_domain::all() noexcept
{
	return numerical_type_domain(detail::numerical_type_domain_all_bits());
}

XMIPP4_INLINE_CONSTEXPR
numerical_type_domain numerical_type_domain::none() noexcept
{
	return numerical_type_domain();
}

XMIPP4_INLINE_CONSTEXPR
bool operator==(
	const numerical_type_domain &lhs,
	const numerical_type_domain &rhs
) noexcept
{
	return lhs.get_mask() == rhs.get_mask();
}

XMIPP4_INLINE_CONSTEXPR
bool operator!=(
	const numerical_type_domain &lhs,
	const numerical_type_domain &rhs
) noexcept
{
	return !(lhs == rhs);
}

XMIPP4_INLINE_CONSTEXPR
numerical_type_domain operator&(
	const numerical_type_domain &lhs,
	const numerical_type_domain &rhs
) noexcept
{
	return numerical_type_domain(lhs.get_mask() & rhs.get_mask());
}

XMIPP4_INLINE_CONSTEXPR
numerical_type_domain operator|(
	const numerical_type_domain &lhs,
	const numerical_type_domain &rhs
) noexcept
{
	return numerical_type_domain(lhs.get_mask() | rhs.get_mask());
}

XMIPP4_INLINE_CONSTEXPR
numerical_type_domain operator~(const numerical_type_domain &domain) noexcept
{
	return numerical_type_domain(
		~domain.get_mask() & detail::numerical_type_domain_all_bits()
	);
}



namespace detail
{

template <typename... Domains>
inline
numerical_type_domain compute_domain_union() noexcept
{
	auto result = numerical_type_domain::none();
	(void) std::initializer_list<int>
	{
		((result = result | Domains::get()), 0)...
	};
	return result;
}

template <typename... Domains>
inline
numerical_type_domain compute_domain_intersection() noexcept
{
	auto result = numerical_type_domain::all();
	(void) std::initializer_list<int>
	{
		((result = result & Domains::get()), 0)...
	};
	return result;
}

} // namespace detail

template <numerical_type_category Category>
inline
const numerical_type_domain& category_type_domain<Category>::get() noexcept
{
	static const numerical_type_domain instance =
		make_numerical_type_domain(Category);
	return instance;
}

template <typename... Domains>
inline
const numerical_type_domain& domain_union<Domains...>::get() noexcept
{
	static_assert(
		sizeof...(Domains) > 0,
		"domain_union requires at least one domain. An empty union would "
		"silently admit no type at all."
	);

	static const numerical_type_domain instance =
		detail::compute_domain_union<Domains...>();
	return instance;
}

template <typename... Domains>
inline
const numerical_type_domain& domain_intersection<Domains...>::get() noexcept
{
	static_assert(
		sizeof...(Domains) > 0,
		"domain_intersection requires at least one domain. An empty "
		"intersection would silently admit every type."
	);

	static const numerical_type_domain instance =
		detail::compute_domain_intersection<Domains...>();
	return instance;
}

inline
const numerical_type_domain& any_type_domain::get() noexcept
{
	static const numerical_type_domain instance =
		numerical_type_domain::all();
	return instance;
}

} // namespace xmipp4
