// SPDX-License-Identifier: GPL-3.0-only

#include "const_any_reference.hpp"

namespace xmipp4 
{

inline
const_any_reference::const_any_reference() noexcept
    : m_type(typeid(void))
    , m_reference(nullptr)
{
}

template <typename T>
inline
const_any_reference::const_any_reference(const T &value) noexcept
    : m_type(typeid(T))
    , m_reference(&value)
{

}

inline
std::type_index const_any_reference::get_type() const noexcept
{
    return m_type;
}

template <typename T>
inline
const T& const_any_reference::get_value() const
{
    if (m_type != typeid(T) || !m_reference)
    {
        throw std::bad_cast("Bad access in const_any_reference");
    }

    return *(static_cast<const T*>(m_reference));
}

} // namespace xmipp4
