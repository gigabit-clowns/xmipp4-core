// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <string>
#include <functional>
#include <cstddef>

namespace xmipp4 
{
namespace multidimensional
{

// TODO string based
class operation_id
{
public:
    explicit operation_id(const std::string &name);
    explicit operation_id(std::string &&name);
    operation_id(const operation_id &other) = default;
    operation_id(operation_id &&other) = default;
    ~operation_id() = default;

    operation_id& operator=(const operation_id &other) = default;
    operation_id& operator=(operation_id &&other) = default;

    const std::string & get_name() const noexcept;

private:
    std::string m_name;

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
