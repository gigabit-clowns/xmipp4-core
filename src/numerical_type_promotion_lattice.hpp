// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/numerical_type.hpp>
#include <xmipp4/core/platform/constexpr.hpp>

#include <array>
#include <bitset>

namespace xmipp4
{

class numerical_type_promotion_lattice
{
public:
	XMIPP4_CONSTEXPR
	numerical_type_promotion_lattice() noexcept;

	XMIPP4_CONSTEXPR
	numerical_type_promotion_lattice& 
	add_edge(numerical_type from, numerical_type to) noexcept;

	XMIPP4_CONSTEXPR
	numerical_type 
	get_supremum(numerical_type a, numerical_type b) const noexcept;

private:
	using reach_set = 
		std::bitset<static_cast<std::size_t>(numerical_type::count)>;

	std::array<
		reach_set, 
		static_cast<std::size_t>(numerical_type::count)
	> m_reach;

	XMIPP4_CONSTEXPR
	bool is_supremum(const reach_set &common, std::size_t i) const noexcept;
};

} // namespace xmipp4

#include "numerical_type_promotion_lattice.inl"
