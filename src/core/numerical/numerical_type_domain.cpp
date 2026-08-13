// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/numerical/numerical_type_domain.hpp>

namespace xmipp4
{

numerical_type_domain make_numerical_type_domain(
	numerical_type_category category
) noexcept
{
	numerical_type_domain::mask_type mask = 0;

	const auto count = static_cast<int>(numerical_type::count);
	for (int i = 0; i < count; ++i)
	{
		const auto type = static_cast<numerical_type>(i);
		if (get_category(type) == category)
		{
			mask |= detail::numerical_type_domain_bit(type);
		}
	}

	return numerical_type_domain(mask);
}

std::ostream& operator<<(
	std::ostream &os,
	const numerical_type_domain &domain
)
{
	if (domain.empty())
	{
		return os << "<none>";
	}

	bool first = true;
	const auto count = static_cast<int>(numerical_type::count);
	for (int i = 0; i < count; ++i)
	{
		const auto type = static_cast<numerical_type>(i);
		if (domain.contains(type))
		{
			if (!first)
			{
				os << ", ";
			}
			os << type;
			first = false;
		}
	}

	return os;
}

} // namespace xmipp4
