// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/multidimensional/operation_id.hpp>
#include <xmipp4/core/compute/device_backend.hpp>

namespace xmipp4 
{
namespace multidimensional
{

class kernel_key
{
public:
    kernel_key(const operation_id &operation_id, 
               const compute::device_backend *backend ) noexcept;
    kernel_key(const kernel_key &other) = default;
    kernel_key(kernel_key &&other) = default;
    ~kernel_key() = default;

    kernel_key& operator=(const kernel_key &other) = default;
    kernel_key& operator=(kernel_key &&other) = default;
    
    const operation_id& get_operation_id() const noexcept;
    const compute::device_backend * get_backend() const noexcept;

private:
    operation_id m_operation_id;
    const compute::device_backend *m_backend;

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
