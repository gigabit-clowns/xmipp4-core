// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <typeindex>
#include <functional>
#include <cstddef>

namespace xmipp4 
{
namespace multidimensional
{

/**
 * @brief Unique identification for an operation.
 * 
 */
class operation_id
{
public:
    explicit operation_id(std::type_index id) noexcept;
    operation_id(const operation_id &other) = default;
    operation_id(operation_id &&other) = default;
    ~operation_id() = default;

    operation_id& operator=(const operation_id &other) = default;
    operation_id& operator=(operation_id &&other) = default;
	
	bool operator==(const operation_id &other) const noexcept;
	bool operator!=(const operation_id &other) const noexcept;

	std::size_t hash() const noexcept;

private:
	std::type_index m_id;
};

} // namespace multidimensional
} // namespace xmipp4

namespace std
{

template<>
struct hash<xmipp4::multidimensional::operation_id>
{
    using key_type = xmipp4::multidimensional::operation_id;
    std::size_t operator()(const key_type &k) const noexcept;
};

} // namespace std

#include "operation_id.inl"
