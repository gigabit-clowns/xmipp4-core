// SPDX-License-Identifier: GPL-3.0-only

#include "kernel_key.hpp"

namespace xmipp4 
{
namespace multidimensional
{

inline
kernel_key::kernel_key(std::type_index tag_type) noexcept
    : m_tag_type(tag_type)
{
}

inline
std::type_index kernel_key::get_tag_type() const noexcept
{
    return m_tag_type;
}

template <typename Tag>
inline
kernel_key kernel_key::from_tag() noexcept
{
    return kernel_key(typeid(Tag));
}



inline
bool operator==(const kernel_key &lhs, const kernel_key &rhs) noexcept
{
    return lhs.get_tag_type() == rhs.get_tag_type();
}

inline
bool operator!=(const kernel_key &lhs, const kernel_key &rhs) noexcept
{
    return !(lhs == rhs);
}

} // namespace multidimensional
} // namespace xmipp4

namespace std
{

inline
std::size_t
hash<xmipp4::multidimensional::kernel_key>::operator()
(const xmipp4::multidimensional::kernel_key &key) const noexcept
{
    const hash<type_index> type_hasher;
    return type_hasher(key.get_tag_type());
}

} // namespace std
