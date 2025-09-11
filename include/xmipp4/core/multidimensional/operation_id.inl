// SPDX-License-Identifier: GPL-3.0-only

#include "operation_id.hpp"

namespace xmipp4 
{
namespace multidimensional
{

inline
operation_id::operation_id(const std::string &name)
    : m_name(name)
{
}

inline
operation_id::operation_id(std::string &&name)
    : m_name(std::move(name))
{
}

inline
const std::string & operation_id::get_name() const noexcept
{
    return m_name;
}



inline
bool operator==(const operation_id &lhs, const operation_id &rhs) noexcept
{
    return lhs.get_name() == rhs.get_name();
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
    return hash<string>()(key.get_name());
}

} // namespace std
