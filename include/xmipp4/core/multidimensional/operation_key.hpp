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

class operation_key
{
public:
    operation_key(const operation_key &other) = default;
    operation_key(operation_key &&other) = default;
    ~operation_key() = default;

    operation_key& operator=(const operation_key &other) = default;
    operation_key& operator=(operation_key &&other) = default;

    std::type_index get_tag_type() const noexcept;

    template <typename Tag>
    static operation_key from_tag() noexcept;

private:
    explicit operation_key(std::type_index tag_type) noexcept;

    std::type_index m_tag_type;

};

bool operator==(const operation_key &lhs, const operation_key &rhs) noexcept;
bool operator!=(const operation_key &lhs, const operation_key &rhs) noexcept;

} // namespace multidimensional
} // namespace xmipp4

namespace std
{

template<typename Key>
struct hash;

template<>
struct hash<xmipp4::multidimensional::operation_key>
{
    using key_type = xmipp4::multidimensional::operation_key;
    std::size_t operator()(const key_type &k) const noexcept;

};

} // namespace std

#include "operation_key.inl"
