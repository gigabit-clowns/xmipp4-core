// SPDX-License-Identifier: GPL-3.0-only

#include "operation_id.hpp"

namespace xmipp4 
{
namespace multidimensional
{

inline
operation_id::operation_id(std::type_index id) noexcept
	: m_id(id)
{
}

inline
bool operation_id::operator==(const operation_id &other) const noexcept
{
	return m_id == other.m_id;
}

inline
bool operation_id::operator!=(const operation_id &other) const noexcept
{
	return !(operator==(other));
}

inline
std::size_t operation_id::hash() const noexcept
{
	return std::hash<std::type_index>()(m_id);
}

template<typename T>
inline
operation_id operation_id::of() noexcept
{
	using operation_type = typename std::decay<T>::type;
	return operation_id(typeid(operation_type));
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
	return key.hash();
}

} // namespace std
