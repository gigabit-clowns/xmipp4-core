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

class operation_id
{
public:
    operation_id(const operation_id &other) = default;
    operation_id(operation_id &&other) = default;
    ~operation_id() = default;

    operation_id& operator=(const operation_id &other) = default;
    operation_id& operator=(operation_id &&other) = default;

    std::type_index get_tag_type() const noexcept;

    template <typename Tag>
    static operation_id from_tag() noexcept;

private:
    explicit operation_id(std::type_index tag_type) noexcept;

    std::type_index m_tag_type;

};

bool operator==(const operation_id &lhs, const operation_id &rhs) noexcept;
bool operator!=(const operation_id &lhs, const operation_id &rhs) noexcept;

} // namespace multidimensional
} // namespace xmipp4

namespace std
{

template<typename Key>
struct hash;

template<>
struct hash<xmipp4::multidimensional::operation_id>
{
    using key_type = xmipp4::multidimensional::operation_id;
    std::size_t operator()(const key_type &k) const noexcept;

};

} // namespace std

#include "operation_id.inl"
