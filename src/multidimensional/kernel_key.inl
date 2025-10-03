// SPDX-License-Identifier: GPL-3.0-only

#include "kernel_key.hpp"

namespace xmipp4 
{
namespace multidimensional
{

inline
kernel_key::kernel_key(const operation_id &operation_id, 
                       const compute::device_backend *backend) noexcept
    : m_operation_id(operation_id)
    , m_backend(backend)
{
}

inline
const operation_id& kernel_key::get_operation_id() const noexcept
{
    return m_operation_id;
}

inline
const compute::device_backend * kernel_key::get_backend() const noexcept
{
    return m_backend;
}



inline
bool operator==(const kernel_key &lhs, const kernel_key &rhs) noexcept
{
    return (lhs.get_operation_id() == rhs.get_operation_id()) &&
           (lhs.get_backend() == rhs.get_backend());
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
    const auto h1 = 
        std::hash<xmipp4::multidimensional::operation_id>()(key.get_operation_id());
    const auto h2 = 
        std::hash<const xmipp4::compute::device_backend *>()(key.get_backend());

    return h1 ^ h2;
}

} // namespace std
