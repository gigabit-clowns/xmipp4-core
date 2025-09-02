// SPDX-License-Identifier: GPL-3.0-only

#include "operation_id.hpp"

namespace xmipp4 
{
namespace multidimensional
{

inline
operation_id::operation_id(std::type_index tag_type) noexcept
    : m_tag_type(tag_type)
{
}

inline
std::type_index operation_id::get_tag_type() const noexcept
{
    return m_tag_type;
}

template <typename Tag>
inline
operation_id operation_id::from_tag() noexcept
{
    return operation_id(typeid(Tag));
}



inline
bool operator==(const operation_id &lhs, const operation_id &rhs) noexcept
{
    return lhs.get_tag_type() == rhs.get_tag_type();
}

inline
bool operator!=(const operation_id &lhs, const operation_id &rhs) noexcept
{
    return !(lhs == rhs);
}

} // namespace multidimensional
} // namespace xmipp4

namespace std
{

inline
std::size_t
hash<xmipp4::multidimensional::operation_id>::operator()
(const xmipp4::multidimensional::operation_id &key) const noexcept
{
    const hash<type_index> type_hasher;
    return type_hasher(key.get_tag_type());
}

} // namespace std
