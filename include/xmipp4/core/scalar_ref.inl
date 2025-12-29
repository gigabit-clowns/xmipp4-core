// SPDX-License-Identifier: GPL-3.0-only

#include "scalar_ref.hpp"

#include "numerical_type_traits.hpp"

namespace xmipp4
{

template <typename T>
XMIPP4_INLINE_CONSTEXPR
scalar_ref::scalar_ref(const T &value) noexcept
	: m_data(&value)
	, m_data_type(numerical_type_of<T>::value)
{
}

template <typename T>
XMIPP4_INLINE_CONSTEXPR
const T& scalar_ref::get() const
{
	if (numerical_type_of<T>::value != m_data_type)
	{
		throw bad_scalar_ref_access();
	}

	return *static_cast<const T*>(m_data);
}

XMIPP4_INLINE_CONSTEXPR
numerical_type scalar_ref::get_data_type() const noexcept
{
	return m_data_type;
}

} // namespace xmipp4
