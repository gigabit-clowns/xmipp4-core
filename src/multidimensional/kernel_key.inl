// SPDX-License-Identifier: GPL-3.0-only

#include "kernel_key.hpp"

namespace xmipp4 
{
namespace multidimensional
{

inline
kernel_key::kernel_key(const std::type_info &operation_key, 
                       const compute::device_backend *backend_key ) noexcept
    : m_operation_key(operation_key)
    , m_backend_key(backend_key)
{
}

inline
std::type_index kernel_key::get_operation_key() const noexcept
{
    return m_operation_key;
}

inline
const compute::device_backend *
kernel_key::get_backend_key() const noexcept
{
    return m_backend_key;
}

inline
bool operator==(const kernel_key &lhs, const kernel_key &rhs) noexcept
{
    return lhs.get_operation_key() == rhs.get_operation_key() &&
           lhs.get_backend_key() == rhs.get_backend_key();
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
    const hash<type_index> operation_hasher;
    const hash<const xmipp4::compute::device_backend *> backend_hasher;

    return
        operation_hasher(key.get_operation_key()) ^
        backend_hasher(key.get_backend_key());
}

} // namespace std
