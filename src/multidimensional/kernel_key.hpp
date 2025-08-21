// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <typeinfo>
#include <typeindex>
#include <functional>
#include <cstddef>

namespace xmipp4 
{
namespace compute
{

class device_backend; 

} // namespace compute

namespace multidimensional
{

class kernel_key
{
public:
    kernel_key(const std::type_info &operation_key, 
               const compute::device_backend *backend_key ) noexcept;
    kernel_key(const kernel_key &other) = default;
    kernel_key(kernel_key &&other) = default;
    ~kernel_key() = default;

    kernel_key& operator=(const kernel_key &other) = default;
    kernel_key& operator=(kernel_key &&other) = default;

    std::type_index get_operation_key() const noexcept;
    const compute::device_backend * get_backend_key() const noexcept;

private:
    std::type_index m_operation_key;
    const compute::device_backend *m_backend_key;

};

bool operator==(const kernel_key &lhs, const kernel_key &rhs) noexcept;
bool operator!=(const kernel_key &lhs, const kernel_key &rhs) noexcept;

} // namespace multidimensional
} // namespace xmipp4

namespace std
{

template<typename Key>
struct hash;

template<>
struct hash<xmipp4::multidimensional::kernel_key>
{
    using key_type = xmipp4::multidimensional::kernel_key;
    std::size_t operator()(const key_type &k) const noexcept;

};

} // namespace std

#include "kernel_key.inl"
