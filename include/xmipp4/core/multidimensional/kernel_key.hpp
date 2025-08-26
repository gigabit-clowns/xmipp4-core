// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <typeinfo>
#include <typeindex>
#include <functional>
#include <cstddef>

namespace xmipp4 
{
namespace multidimensional
{

class kernel_key
{
public:
    kernel_key(const kernel_key &other) = default;
    kernel_key(kernel_key &&other) = default;
    ~kernel_key() = default;

    kernel_key& operator=(const kernel_key &other) = default;
    kernel_key& operator=(kernel_key &&other) = default;

    std::type_index get_tag_type() const noexcept;

    template <typename Tag>
    static kernel_key from_tag() noexcept;

private:
    explicit kernel_key(std::type_index tag_type) noexcept;

    std::type_index m_tag_type;

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
